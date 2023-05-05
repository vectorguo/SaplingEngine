#include "Dx12CommandManager.h"
#include "GraphicsManager.h"

#include "Runtime/Component/Renderer/Renderer.h"
#include "Runtime/Render/Graphics/Material/Material.h"
#include "Runtime/Render/Graphics/Mesh/Mesh.h"

namespace SaplingEngine
{
	ComPtr<ID3D12CommandQueue>			Dx12CommandManager::m_CommandQueue;
	ComPtr<ID3D12CommandAllocator>		Dx12CommandManager::m_CommandAllocator;
	ComPtr<ID3D12GraphicsCommandList>	Dx12CommandManager::m_CommandList;
	ComPtr<ID3D12Fence>					Dx12CommandManager::m_Fence;
	uint64_t							Dx12CommandManager::m_CurrentFence = 0;

	/**
	 * \brief 开始初始化
	 */
	void Dx12CommandManager::BeginInitialize()
	{
		//获取Dx12 Device指针
		auto* pDx12Device = GraphicsManager::GetDx12Device();
		
		//创建围栏
		ThrowIfFailed(pDx12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));

		//创建命令队列/命令分配器和命令列表
		D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
		commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		ThrowIfFailed(pDx12Device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_CommandQueue)));
		ThrowIfFailed(pDx12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator)));
		ThrowIfFailed(pDx12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator.Get(), nullptr, IID_PPV_ARGS(m_CommandList.GetAddressOf())));
		m_CommandList->Close();
		
		//开启命令列表
		ThrowIfFailed(m_CommandList->Reset(m_CommandAllocator.Get(), nullptr));
	}

	/**
	 * \brief 结束初始化
	 */
	void Dx12CommandManager::EndInitialize()
	{
		ExecuteCommandList();
		CompleteCommand();
	}

	/**
	 * \brief 执行渲染前的准备工作
	 */
	void Dx12CommandManager::PreRender()
	{
		//重置命令
		ThrowIfFailed(m_CommandAllocator->Reset());
		ThrowIfFailed(m_CommandList->Reset(m_CommandAllocator.Get(), nullptr));
	}

	/**
	 * \brief 执行渲染后的清理工作
	 */
	void Dx12CommandManager::PostRender()
	{
		ExecuteCommandList();

		//交换后台缓冲和前台缓冲
		GraphicsManager::Present();

		//等待命令结束
		CompleteCommand();
	}

	/**
	 * \brief 销毁
	 */
	void Dx12CommandManager::Destroy()
	{
		CompleteCommand();
	}

	/**
	 * \brief 绘制物体
	 * \param pRenderer renderer
	 */
	void Dx12CommandManager::DrawIndexedInstanced(const Renderer* pRenderer)
	{
		const auto* pMesh = pRenderer->GetMesh();
		m_CommandList->IASetVertexBuffers(0, 1, pMesh->GetVertexBufferView());
		m_CommandList->IASetIndexBuffer(pMesh->GetIndexBufferView());
		m_CommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		const auto* pMaterial = pRenderer->GetMaterial();
		const auto& textures = pMaterial->GetTextures();
		for (auto i = 0; i < textures.size(); ++i)
		{
			m_CommandList->SetGraphicsRootDescriptorTable(4 + i, textures[i]->GetSrvDescriptor());
		}

		m_CommandList->SetGraphicsRootConstantBufferView(0, pRenderer->GetCommonCbAddress());
		m_CommandList->SetGraphicsRootConstantBufferView(1, pRenderer->GetSpecialCbAddress());
		m_CommandList->DrawIndexedInstanced(pMesh->GetIndexCount(), 1, 0, 0, 0);
	}

	/**
	 * \brief 为ShadowMap绘制物体
	 * \param pRenderer renderer
	 */
	void Dx12CommandManager::DrawIndexedInstancedForShadowMap(const Renderer* pRenderer)
	{
		const auto* pMesh = pRenderer->GetMesh();
		m_CommandList->IASetVertexBuffers(0, 1, pMesh->GetVertexBufferView());
		m_CommandList->IASetIndexBuffer(pMesh->GetIndexBufferView());
		m_CommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_CommandList->SetGraphicsRootConstantBufferView(0, pRenderer->GetCommonCbAddress());
		m_CommandList->SetGraphicsRootConstantBufferView(1, pRenderer->GetSpecialCbAddress());
		m_CommandList->DrawIndexedInstanced(pMesh->GetIndexCount(), 1, 0, 0, 0);
	}

	/**
	 * \brief 执行命令
	 */
	void Dx12CommandManager::ExecuteCommandList()
	{
		//完成并执行渲染命令
		ThrowIfFailed(m_CommandList->Close());
		ID3D12CommandList* commands[] = { m_CommandList.Get() };
		m_CommandQueue->ExecuteCommandLists(_countof(commands), commands);
	}

	/**
	 * \brief 等待命令完成
	 */
	void Dx12CommandManager::CompleteCommand()
	{
		++m_CurrentFence;
		ThrowIfFailed(m_CommandQueue->Signal(m_Fence.Get(), m_CurrentFence));

		if (m_Fence->GetCompletedValue() < m_CurrentFence)
		{
			HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
			ThrowIfFailed(m_Fence->SetEventOnCompletion(m_CurrentFence, eventHandle));
			WaitForSingleObject(eventHandle, INFINITE);
			CloseHandle(eventHandle);
		}
	}
}

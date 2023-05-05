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
	 * \brief ��ʼ��ʼ��
	 */
	void Dx12CommandManager::BeginInitialize()
	{
		//��ȡDx12 Deviceָ��
		auto* pDx12Device = GraphicsManager::GetDx12Device();
		
		//����Χ��
		ThrowIfFailed(pDx12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));

		//�����������/����������������б�
		D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
		commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		ThrowIfFailed(pDx12Device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_CommandQueue)));
		ThrowIfFailed(pDx12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator)));
		ThrowIfFailed(pDx12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator.Get(), nullptr, IID_PPV_ARGS(m_CommandList.GetAddressOf())));
		m_CommandList->Close();
		
		//���������б�
		ThrowIfFailed(m_CommandList->Reset(m_CommandAllocator.Get(), nullptr));
	}

	/**
	 * \brief ������ʼ��
	 */
	void Dx12CommandManager::EndInitialize()
	{
		ExecuteCommandList();
		CompleteCommand();
	}

	/**
	 * \brief ִ����Ⱦǰ��׼������
	 */
	void Dx12CommandManager::PreRender()
	{
		//��������
		ThrowIfFailed(m_CommandAllocator->Reset());
		ThrowIfFailed(m_CommandList->Reset(m_CommandAllocator.Get(), nullptr));
	}

	/**
	 * \brief ִ����Ⱦ���������
	 */
	void Dx12CommandManager::PostRender()
	{
		ExecuteCommandList();

		//������̨�����ǰ̨����
		GraphicsManager::Present();

		//�ȴ��������
		CompleteCommand();
	}

	/**
	 * \brief ����
	 */
	void Dx12CommandManager::Destroy()
	{
		CompleteCommand();
	}

	/**
	 * \brief ��������
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
	 * \brief ΪShadowMap��������
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
	 * \brief ִ������
	 */
	void Dx12CommandManager::ExecuteCommandList()
	{
		//��ɲ�ִ����Ⱦ����
		ThrowIfFailed(m_CommandList->Close());
		ID3D12CommandList* commands[] = { m_CommandList.Get() };
		m_CommandQueue->ExecuteCommandLists(_countof(commands), commands);
	}

	/**
	 * \brief �ȴ��������
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

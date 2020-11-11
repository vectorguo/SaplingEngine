#include "Dx12CommandManager.h"
#include "Dx12GraphicsManager.h"
#include "Render/Graphics/Material.h"
#include "Render/Graphics/Mesh.h"
#include "Render/Renderer/Renderer.h"

namespace SaplingEngine
{
	Dx12CommandManager* Dx12CommandManager::m_Instance = nullptr;
	
	/**
	 * \brief ��ʼ��ʼ��
	 */
	void Dx12CommandManager::BeginInitialize()
	{
		m_pGraphicsManager = GraphicsManager::Instance();
		m_pGraphicsManager->m_pCommandManager = this;

		//��ȡDx12 Deviceָ��
		auto* pDx12Device = m_pGraphicsManager->GetDx12Device();
		
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

		//����ViewPort��ScissorRect
		m_CommandList->RSSetViewports(1, &m_pGraphicsManager->m_Viewport);
		m_CommandList->RSSetScissorRects(1, &m_pGraphicsManager->m_ScissorRect);

		//��Ⱦ����ӳ���״̬�л���RT״̬
		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_pGraphicsManager->GetCurrentRt(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_CommandList->ResourceBarrier(1, &resourceBarrier);
	}

	/**
	 * \brief ִ����Ⱦ���������
	 */
	void Dx12CommandManager::PostRender()
	{
		//��Ⱦ�����RT״̬�л�������״̬
		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_pGraphicsManager->GetCurrentRt(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_CommandList->ResourceBarrier(1, &resourceBarrier);
		ExecuteCommandList();

		//������̨�����ǰ̨����
		m_pGraphicsManager->Present();

		//�ȴ��������
		CompleteCommand();

		//����Pipeline״̬����
		m_CurrentPipelineStateName.clear();
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
		const auto* pMaterial = pRenderer->GetMaterial();
		const auto& pipelineStateName = pMaterial->GetShaderName();
		if (m_CurrentPipelineStateName != pipelineStateName)
		{
			//��Ҫ�л���Ⱦ����״̬
			m_CommandList->SetPipelineState(m_pGraphicsManager->GetPipelineState(pipelineStateName));
			m_CurrentPipelineStateName = pipelineStateName;
		}
		
		const auto* pMesh = pRenderer->GetMesh();
		m_CommandList->IASetVertexBuffers(0, 1, pMesh->GetVertexBufferView());
		m_CommandList->IASetIndexBuffer(pMesh->GetIndexBufferView());
		m_CommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//��ȡ��Renderer����Ӧ�ĳ���������������
		m_CommandList->SetGraphicsRootDescriptorTable(0, CBufferManager::Instance()->GetObjectCbvDescriptor(pRenderer->GetCommonOcbIndex()));
		m_CommandList->SetGraphicsRootDescriptorTable(1, CBufferManager::Instance()->GetObjectCbvDescriptor(pRenderer->GetSpecialOcbIndex()));
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
		m_CurrentFence++;
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

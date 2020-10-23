// ReSharper disable CppMemberFunctionMayBeConst
#pragma once

#include "Dx12GraphicsPch.h"
#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Dx12CommandManager final
	{
		friend class RenderPipeline;
		
	public:
		Dx12CommandManager();
		~Dx12CommandManager();

		Dx12CommandManager(const Dx12CommandManager&) = delete;
		Dx12CommandManager(Dx12CommandManager&&) = delete;
		Dx12CommandManager& operator=(const Dx12CommandManager&) = delete;
		Dx12CommandManager& operator=(Dx12CommandManager&&) = delete;

		/**
		 * \brief ��ȡ����
		 * \return ����
		 */
		static Dx12CommandManager* Instance()
		{
			return m_Instance;
		}

		/**
		 * \brief ��ȡ�����б�
		 * \return �����б�
		 */
		ID3D12GraphicsCommandList* GetCommandList() const
		{
			return m_CommandList.Get();
		}

		/**
		 * \brief ��ȡ�����б�
		 * \return �����б�
		 */
		ID3D12CommandQueue* GetCommandQueue() const
		{
			return m_CommandQueue.Get();
		}
		
		/**
		 * \brief ������Դת��
		 * \param pResource ��Ҫ����ת������Դ
		 * \param stateBefore ת��ǰ��״̬
		 * \param stateAfter ת�����״̬
		 */
		void CacheResourceBarrierTransition(ID3D12Resource* pResource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter)
		{
			const auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(pResource, stateBefore, stateAfter);
			m_ResourceBarrierTransitions.push_back(resourceBarrier);
		}
		
		void ResourceBarrierTransition(ID3D12Resource* pResource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter)
		{
			auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(pResource, stateBefore, stateAfter);
			m_CommandList->ResourceBarrier(1, &resourceBarrier);
		}
		
		void SetViewports(const D3D12_VIEWPORT* pViewports)
		{
			m_CommandList->RSSetViewports(1, pViewports);
		}

		void SetScissorRects(const D3D12_RECT* pRects)
		{
			m_CommandList->RSSetScissorRects(1, pRects);
		}
		
	private:
		/**
		 * \brief ��ʼ��
		 * \param pDx12Device Dx12�豸
		 */
		void Initialize(ID3D12Device* pDx12Device);

		/**
		 * \brief ����
		 */
		void Destroy();

		/**
		 * \brief �������������
		 */
		void ResetCommandAllocator()
		{
			ThrowIfFailed(m_CommandAllocator->Reset());
		}

		/**
		 * \brief ִ������
		 */
		void ExecuteCommandList();

		/**
		 * \brief ���������б�
		 */
		void ResetCommandList()
		{
			ThrowIfFailed(m_CommandList->Reset(m_CommandAllocator.Get(), nullptr));
		}

		void ResetCommandList(ID3D12PipelineState* pPipelineState)
		{
			ThrowIfFailed(m_CommandList->Reset(m_CommandAllocator.Get(), pPipelineState));
		}

		/**
		 * \brief �ȴ��������
		 */
		void CompleteCommand();
		
	private:
		/**
		 * \brief ����
		 */
		static Dx12CommandManager* m_Instance;
		
		ComPtr<ID3D12CommandQueue> m_CommandQueue;
		ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;

		ComPtr<ID3D12Fence> m_Fence;
		uint64_t m_CurrentFence = 0;

		std::vector<CD3DX12_RESOURCE_BARRIER> m_ResourceBarrierTransitions;
	};
}

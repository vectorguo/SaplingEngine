// ReSharper disable CppMemberFunctionMayBeConst
#pragma once

#include "RenderPipeline/GraphicsPch.h"
#include "RenderPipeline/CommandManager.h"
#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Dx12GraphicsManager;
	
	class Dx12CommandManager final : public CommandManager
	{
		friend class Dx12GraphicsManager;
		
	public:
		Dx12CommandManager() = default;
		~Dx12CommandManager() override = default;

		Dx12CommandManager(const Dx12CommandManager&) = delete;
		Dx12CommandManager(Dx12CommandManager&&) = delete;
		Dx12CommandManager& operator=(const Dx12CommandManager&) = delete;
		Dx12CommandManager& operator=(Dx12CommandManager&&) = delete;

		/**
		 * \brief ��ʼ��ʼ��
		 */
		void BeginInitialize() override;

		/**
		 * \brief ������ʼ��
		 */
		void EndInitialize() override;

		/**
		 * \brief ִ����Ⱦǰ��׼������
		 */
		void PreRender() override;

		/**
		 * \brief ִ����Ⱦ���������
		 */
		void PostRender() override;

		/**
		 * \brief ����
		 */
		void Destroy() override;
		
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
		void ResourceBarrierTransition(ID3D12Resource* pResource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter)
		{
			auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(pResource, stateBefore, stateAfter);
			m_CommandList->ResourceBarrier(1, &resourceBarrier);
		}
		
	private:
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
		Dx12GraphicsManager* m_pGraphicsManager = nullptr;
		
		ComPtr<ID3D12CommandQueue> m_CommandQueue;
		ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;

		ComPtr<ID3D12Fence> m_Fence;
		uint64_t m_CurrentFence = 0;
	};
}

// ReSharper disable CppMemberFunctionMayBeConst
#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Renderer;
	class Dx12GraphicsManager;
	
	class Dx12CommandManager final
	{
		friend class Dx12GraphicsManager;
		
	public:
		Dx12CommandManager()
		{
			m_Instance = this;
		}
		~Dx12CommandManager() = default;

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
		 * \brief ��ʼ��ʼ��
		 */
		void BeginInitialize();

		/**
		 * \brief ������ʼ��
		 */
		void EndInitialize();

		/**
		 * \brief ִ����Ⱦǰ��׼������
		 */
		void PreRender();

		/**
		 * \brief ִ����Ⱦ���������
		 */
		void PostRender();

		/**
		 * \brief ����
		 */
		void Destroy();
		
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
		 * \brief ��������
		 * \param pRenderer renderer
		 */
		void DrawIndexedInstanced(const Renderer* pRenderer);
		
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
		 * \brief �ȴ��������
		 */
		void CompleteCommand();
		
	private:
		/**
		 * \brief ����
		 */
		static Dx12CommandManager* m_Instance;
		
		Dx12GraphicsManager* m_pGraphicsManager = nullptr;
		
		ComPtr<ID3D12CommandQueue> m_CommandQueue;
		ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;

		ComPtr<ID3D12Fence> m_Fence;
		uint64_t m_CurrentFence = 0;

		/**
		 * \brief ��ǰ��Ⱦ����״̬����
		 */
		std::string m_CurrentPipelineStateName;
	};

	using CommandManager = Dx12CommandManager;
}

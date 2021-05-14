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
		/**
		 * \brief ��ʼ��ʼ��
		 */
		static void BeginInitialize();

		/**
		 * \brief ������ʼ��
		 */
		static void EndInitialize();

		/**
		 * \brief ִ����Ⱦǰ��׼������
		 */
		static void PreRender();

		/**
		 * \brief ִ����Ⱦ���������
		 */
		static void PostRender();

		/**
		 * \brief ����
		 */
		static void Destroy();
		
		/**
		 * \brief ��ȡ�����б�
		 * \return �����б�
		 */
		static ID3D12GraphicsCommandList* GetCommandList()
		{
			return m_CommandList.Get();
		}

		/**
		 * \brief ��ȡ�����б�
		 * \return �����б�
		 */
		static ID3D12CommandQueue* GetCommandQueue()
		{
			return m_CommandQueue.Get();
		}

		/**
		 * \brief ��������
		 * \param pRenderer renderer
		 */
		static void DrawIndexedInstanced(const Renderer* pRenderer);

		/**
		 * \brief ΪShadowMap��������
		 * \param pRenderer renderer
		 */
		static void DrawIndexedInstancedForShadowMap(const Renderer* pRenderer);
		
		/**
		 * \brief ������Դת��
		 * \param pResource ��Ҫ����ת������Դ
		 * \param stateBefore ת��ǰ��״̬
		 * \param stateAfter ת�����״̬
		 */
		static void ResourceBarrierTransition(ID3D12Resource* pResource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter)
		{
			auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(pResource, stateBefore, stateAfter);
			m_CommandList->ResourceBarrier(1, &resourceBarrier);
		}
		
	private:
		/**
		 * \brief ִ������
		 */
		static void ExecuteCommandList();

		/**
		 * \brief �ȴ��������
		 */
		static void CompleteCommand();
		
	private:
		static ComPtr<ID3D12CommandQueue> m_CommandQueue;
		static ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
		static ComPtr<ID3D12GraphicsCommandList> m_CommandList;

		static ComPtr<ID3D12Fence> m_Fence;
		static uint64_t m_CurrentFence;
	};

	using CommandManager = Dx12CommandManager;
}

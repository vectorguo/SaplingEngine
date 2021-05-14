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
		 * \brief 开始初始化
		 */
		static void BeginInitialize();

		/**
		 * \brief 结束初始化
		 */
		static void EndInitialize();

		/**
		 * \brief 执行渲染前的准备工作
		 */
		static void PreRender();

		/**
		 * \brief 执行渲染后的清理工作
		 */
		static void PostRender();

		/**
		 * \brief 销毁
		 */
		static void Destroy();
		
		/**
		 * \brief 获取命令列表
		 * \return 命令列表
		 */
		static ID3D12GraphicsCommandList* GetCommandList()
		{
			return m_CommandList.Get();
		}

		/**
		 * \brief 获取命令列表
		 * \return 命令列表
		 */
		static ID3D12CommandQueue* GetCommandQueue()
		{
			return m_CommandQueue.Get();
		}

		/**
		 * \brief 绘制物体
		 * \param pRenderer renderer
		 */
		static void DrawIndexedInstanced(const Renderer* pRenderer);

		/**
		 * \brief 为ShadowMap绘制物体
		 * \param pRenderer renderer
		 */
		static void DrawIndexedInstancedForShadowMap(const Renderer* pRenderer);
		
		/**
		 * \brief 缓存资源转换
		 * \param pResource 需要进行转换的资源
		 * \param stateBefore 转换前的状态
		 * \param stateAfter 转换后的状态
		 */
		static void ResourceBarrierTransition(ID3D12Resource* pResource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter)
		{
			auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(pResource, stateBefore, stateAfter);
			m_CommandList->ResourceBarrier(1, &resourceBarrier);
		}
		
	private:
		/**
		 * \brief 执行命令
		 */
		static void ExecuteCommandList();

		/**
		 * \brief 等待命令完成
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

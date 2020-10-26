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
		 * \brief 开始初始化
		 */
		void BeginInitialize() override;

		/**
		 * \brief 结束初始化
		 */
		void EndInitialize() override;

		/**
		 * \brief 执行渲染前的准备工作
		 */
		void PreRender() override;

		/**
		 * \brief 执行渲染后的清理工作
		 */
		void PostRender() override;

		/**
		 * \brief 销毁
		 */
		void Destroy() override;
		
		/**
		 * \brief 获取命令列表
		 * \return 命令列表
		 */
		ID3D12GraphicsCommandList* GetCommandList() const
		{
			return m_CommandList.Get();
		}

		/**
		 * \brief 获取命令列表
		 * \return 命令列表
		 */
		ID3D12CommandQueue* GetCommandQueue() const
		{
			return m_CommandQueue.Get();
		}
		
		/**
		 * \brief 缓存资源转换
		 * \param pResource 需要进行转换的资源
		 * \param stateBefore 转换前的状态
		 * \param stateAfter 转换后的状态
		 */
		void ResourceBarrierTransition(ID3D12Resource* pResource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter)
		{
			auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(pResource, stateBefore, stateAfter);
			m_CommandList->ResourceBarrier(1, &resourceBarrier);
		}
		
	private:
		/**
		 * \brief 执行命令
		 */
		void ExecuteCommandList();

		/**
		 * \brief 重置命令列表
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
		 * \brief 等待命令完成
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

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
		 * \brief 获取单例
		 * \return 单例
		 */
		static Dx12CommandManager* Instance()
		{
			return m_Instance;
		}

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
		 * \brief 初始化
		 * \param pDx12Device Dx12设备
		 */
		void Initialize(ID3D12Device* pDx12Device);

		/**
		 * \brief 销毁
		 */
		void Destroy();

		/**
		 * \brief 重置命令分配器
		 */
		void ResetCommandAllocator()
		{
			ThrowIfFailed(m_CommandAllocator->Reset());
		}

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
		/**
		 * \brief 单例
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

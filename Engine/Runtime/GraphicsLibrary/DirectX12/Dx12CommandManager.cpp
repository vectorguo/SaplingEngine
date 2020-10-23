#include "Dx12CommandManager.h"

namespace SaplingEngine
{
	Dx12CommandManager* Dx12CommandManager::m_Instance = nullptr;
	
	Dx12CommandManager::Dx12CommandManager()
	{
		m_Instance = this;
	}
	
	Dx12CommandManager::~Dx12CommandManager()
	{
		m_Instance = nullptr;
	}

	/**
	 * \brief 初始化
	 * \param pDx12Device Dx12设备
	 */
	void Dx12CommandManager::Initialize(ID3D12Device* pDx12Device)
	{
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
	}

	/**
	 * \brief 销毁
	 */
	void Dx12CommandManager::Destroy()
	{
		CompleteCommand();
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

#include "D3D12Application.h"

namespace SaplingEngine
{
	D3D12Application::D3D12Application() : GameApplication(), m_Viewport(), m_ScissorRect()
	{
	}

	D3D12Application::~D3D12Application() = default;

	/**
	 * \brief 初始化配置
	 * \return 是否初始化成功
	 */
	bool D3D12Application::InitializeConfig()
	{
		m_Width = 1280;
		m_Height = 720;
		return true;
	}

	/**
	 * \brief 更新
	 */
	void D3D12Application::Update()
	{
	}

	/**
	 * \brief 绘制
	 */
	void D3D12Application::Render()
	{
		ThrowIfFailed(m_CommandAllocator->Reset());
		ThrowIfFailed(m_CommandList->Reset(m_CommandAllocator.Get(), nullptr));
		
		//切换渲染状态
		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_CommandList->ResourceBarrier(1, &resourceBarrier);

		//设置ViewPort和ScissorRect
		m_CommandList->RSSetViewports(1, &m_Viewport);
		m_CommandList->RSSetScissorRects(1, &m_ScissorRect);

		//清除后台缓冲区和深度模板缓冲区
		const auto rtv = CurrentBackBufferView();
		const auto dsv = DepthStencilBufferView();
		m_CommandList->ClearRenderTargetView(rtv, Color::LightBlue, 0, nullptr);
		m_CommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

		//指定要渲染的缓冲区
		m_CommandList->OMSetRenderTargets(1, &rtv, true, &dsv);

		//渲染物体
		//TODO
		
		//将当前的后台缓冲区从渲染目标状态设置为呈现状态
		resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_CommandList->ResourceBarrier(1, &resourceBarrier);

		//完成并执行渲染命令
		ExecuteCommandList();

		//交换后台缓冲和前台缓冲
		ThrowIfFailed(m_SwapChain->Present(0, 0));
		m_BackBufferIndex = (m_BackBufferIndex + 1) % SwapChainBufferCount;

		FlushCommandQueue();
	}

	/**
	 * \brief 窗口变化的回调函数
	 */
	void D3D12Application::OnResize()
	{
		FlushCommandQueue();

		//Reset Command List
		ThrowIfFailed(m_CommandList->Reset(m_CommandAllocator.Get(), nullptr));

		//Resize SwapChain Buffer
		CreateRenderTargetViews();
		CreateDepthStencilView();

		ExecuteCommandList();
		FlushCommandQueue();

		//Reset ViewPort
		m_Viewport.TopLeftX = 0;
		m_Viewport.TopLeftY = 0;
		m_Viewport.Width = static_cast<float>(m_Width);
		m_Viewport.Height = static_cast<float>(m_Height);
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;

		//Reset ScissorRect
		m_ScissorRect = { 0, 0, static_cast<long>(m_Width), static_cast<long>(m_Height) };
	}
	
	/**
	 * \brief 初始化DirectX12
	 * \return 是否初始化成功
	 */
	bool D3D12Application::InitializeGraphics()
	{
#if defined(DEBUG) || defined(_DEBUG) 
		{// Enable the D3D12 debug layer.
			ComPtr<ID3D12Debug> debugController;
			if (!FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			{
				debugController->EnableDebugLayer();
			}
		}
#endif

		//创建DXGI，用来枚举显卡，创建SwapChain
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_DXGIFactory)));

		//创建显示设备(该设备对应一个显示适配器，比如显卡。下面nullptr代表使用默认的显示适配器)
		const auto result = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_D3D12Device));
		if (FAILED(result))
		{//默认显示适配器对应的设备创建失败
			ComPtr<IDXGIAdapter> pWarpAdapter;
			ThrowIfFailed(m_DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));
			ThrowIfFailed(D3D12CreateDevice(pWarpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_D3D12Device)));
		}

		//创建围栏
		ThrowIfFailed(m_D3D12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));

		//查询描述符大小
		m_RtvDescriptorSize = m_D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		m_DsvDescriptorSize = m_D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		m_CbvDescriptorSize = m_D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		
		//创建命令队列/命令分配器和命令列表
		D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
		commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		ThrowIfFailed(m_D3D12Device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_CommandQueue)));
		ThrowIfFailed(m_D3D12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator)));
		ThrowIfFailed(m_D3D12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator.Get(), nullptr, IID_PPV_ARGS(m_CommandList.GetAddressOf())));
		m_CommandList->Close();

		//创建交换链
		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = m_Width;
		sd.BufferDesc.Height = m_Height;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = m_SwapChainBufferFormat;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = SwapChainBufferCount;
		sd.OutputWindow = m_MainWindow;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		m_DXGIFactory->CreateSwapChain(m_CommandQueue.Get(), &sd, m_SwapChain.GetAddressOf());

		//创建RTV描述符堆和RTV描述符
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		heapDesc.NumDescriptors = SwapChainBufferCount;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NodeMask = 0;
		ThrowIfFailed(m_D3D12Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_RtvDescriptorHeap.GetAddressOf())));

		//创建深度/裁剪描述符的描述符堆
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		heapDesc.NumDescriptors = 1;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NodeMask = 0;
		ThrowIfFailed(m_D3D12Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_DsvDescriptorHeap.GetAddressOf())));

		return true;
	}

	/**
	 * \brief 创建渲染缓冲视图
	 */
	void D3D12Application::CreateRenderTargetViews()
	{
		m_BackBufferIndex = 0;
		
		//释放之前的缓存
		for (auto& buffer : m_SwapChainBuffer)
		{
			buffer.Reset();
		}

		//重新设置交换链Buffer大小
		ThrowIfFailed(m_SwapChain->ResizeBuffers(SwapChainBufferCount, m_Width, m_Height, m_SwapChainBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

		auto rtvHeapHandle = m_RtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		for (auto i = 0; i < SwapChainBufferCount; ++i)
		{
			//首先从已经创建好的交换连SwapChain中获取对应索引的渲染缓冲buffer，填充到交换链缓冲区数组中
			ThrowIfFailed(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_SwapChainBuffer[i])));

			//为渲染缓冲创建渲染目标视图，并存储在渲染视图描述符堆中
			m_D3D12Device->CreateRenderTargetView(m_SwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);

			//获取渲染视图描述符堆中的下一个渲染对象视图的句柄
			rtvHeapHandle.ptr += m_RtvDescriptorSize;
		}
	}

	/**
	 * \brief 创建深度模板缓冲视图
	 */
	void D3D12Application::CreateDepthStencilView()
	{
		//释放之前的缓存
		m_DepthStencilBuffer.Reset();

		//创建深度/模板缓冲区对应的资源描述结构
		D3D12_RESOURCE_DESC depthStencilDesc;
		depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		depthStencilDesc.Alignment = 0;
		depthStencilDesc.Width = m_Width;
		depthStencilDesc.Height = m_Height;
		depthStencilDesc.DepthOrArraySize = 1;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		//设置资源清除选项
		D3D12_CLEAR_VALUE optClear;
		optClear.Format = m_DepthStencilViewFormat;
		optClear.DepthStencil.Depth = 1.0f;
		optClear.DepthStencil.Stencil = 0;

		//创建深度/模板缓冲对应的纹理资源和存储资源的堆，并把创建的资源提交到堆中
		D3D12_HEAP_PROPERTIES heapProperties
		{
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
			D3D12_MEMORY_POOL_UNKNOWN,
			1,1
		};

		ThrowIfFailed(m_D3D12Device->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&depthStencilDesc,
			D3D12_RESOURCE_STATE_COMMON,
			&optClear,
			IID_PPV_ARGS(m_DepthStencilBuffer.GetAddressOf())));

		//创建深度/模板视图
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Format = m_DepthStencilViewFormat;
		dsvDesc.Texture2D.MipSlice = 0;
		m_D3D12Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &dsvDesc, m_DsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

		// Transition the resource from its initial state to be used as a depth buffer.
		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_DepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
		m_CommandList->ResourceBarrier(1, &resourceBarrier);
	}

	/**
	 * \brief 执行命令
	 */
	void D3D12Application::ExecuteCommandList() const
	{
		//完成并执行渲染命令
		ThrowIfFailed(m_CommandList->Close());
		ID3D12CommandList* commands[] = { m_CommandList.Get() };
		m_CommandQueue->ExecuteCommandLists(_countof(commands), commands);
	}

	void D3D12Application::FlushCommandQueue()
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

#include "D3D12Application.h"

namespace SaplingEngine
{
	D3D12Application::D3D12Application() : SaplingEngine::GameApplication()
	{

	}

	D3D12Application::~D3D12Application()
	{
	}

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
	 * \brief 初始化应用程序
	 * \param hInstance 应用程序句柄
	 * \return 是否初始化成功
	 */
	bool D3D12Application::InitializeApplication(HINSTANCE hInstance)
	{
		const auto result = InitializeWindow() && InitializeDirectX12();
		if (result)
		{
			OnResize();
		}
		return result;
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
	}

	/**
	 * \brief 消息处理回调函数
	 */
	LRESULT D3D12Application::MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		{
			return dynamic_cast<D3D12Application*>(Instance())->MessageProcess(hWnd, msg, wParam, lParam);
		}
	}
	
	/**
	 * \brief 初始化窗口
	 * \return 是否初始化成功
	 */
	bool D3D12Application::InitializeWindow()
	{
		WNDCLASS wc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = MainWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_AppInstance;
		wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = L"MainWnd";

		if (!RegisterClass(&wc))
		{
			MessageBox(nullptr, L"RegisterClass Failed.", nullptr, 0);
			return false;
		}

		RECT rect = { 0, 0, m_Width, m_Height };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
		const int width = rect.right - rect.left;
		const int height = rect.bottom - rect.top;

		m_MainWindow = CreateWindow(L"MainWnd", L"Sapling", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, m_AppInstance, nullptr);
		if (!m_MainWindow)
		{
			MessageBox(nullptr, L"CreateWindow Failed.", nullptr, 0);
			return false;
		}

		ShowWindow(m_MainWindow, SW_SHOW);
		UpdateWindow(m_MainWindow);

		return true;
	}

	/**
	 * \brief 初始化DirectX12
	 * \return 是否初始化成功
	 */
	bool D3D12Application::InitializeDirectX12()
	{
#if defined(DEBUG) || defined(_DEBUG) 
		{// Enable the D3D12 debug layer.
			ComPtr<ID3D12Debug> debugController;
			const auto hResult = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
			if (!FAILED(hResult))
			{
				debugController->EnableDebugLayer();
			}
		}
#endif

		//创建DXGI，用来枚举显卡，创建SwapChain
		ReturnIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_DXGIFactory)));

		//创建显示设备(该设备对应一个显示适配器，比如显卡。下面nullptr代表使用默认的显示适配器)
		const auto result = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_D3D12Device));
		if (FAILED(result))
		{//默认显示适配器对应的设备创建失败
			ComPtr<IDXGIAdapter> pWarpAdapter;
			ReturnIfFailed(m_DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));
			ReturnIfFailed(D3D12CreateDevice(pWarpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_D3D12Device)))
		}

		//创建围栏
		ReturnIfFailed(m_D3D12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)))

		//创建命令队列/命令分配器和命令列表
		D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
		commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		ReturnIfFailed(m_D3D12Device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_CommandQueue)));
		ReturnIfFailed(m_D3D12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator)));
		ReturnIfFailed(m_D3D12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_CommandList)))
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
		ReturnIfFailed(m_D3D12Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_RtvDescriptorHeap.GetAddressOf())))

		//创建深度/裁剪描述符的描述符堆
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		heapDesc.NumDescriptors = 1;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NodeMask = 0;
		ReturnIfFailed(m_D3D12Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_DsvDescriptorHeap.GetAddressOf())))

		//查询描述符大小
		m_RtvDescriptorSize = m_D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		m_DsvDescriptorSize = m_D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		m_CbvDescriptorSize = m_D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		
		return true;
	}

	/**
	 * \brief 消息处理
	 */
	LRESULT D3D12Application::MessageProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}

	/**
	 * \brief 窗口变化的回调函数
	 */
	void D3D12Application::OnResize()
	{

	}
}

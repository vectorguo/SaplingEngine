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
	 * \brief ��ʼ������
	 * \return �Ƿ��ʼ���ɹ�
	 */
	bool D3D12Application::InitializeConfig()
	{
		m_Width = 1280;
		m_Height = 720;
		return true;
	}

	/**
	 * \brief ��ʼ��Ӧ�ó���
	 * \param hInstance Ӧ�ó�����
	 * \return �Ƿ��ʼ���ɹ�
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
	 * \brief ����
	 */
	void D3D12Application::Update()
	{
	}

	/**
	 * \brief ����
	 */
	void D3D12Application::Render()
	{
	}

	/**
	 * \brief ��Ϣ����ص�����
	 */
	LRESULT D3D12Application::MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		{
			return dynamic_cast<D3D12Application*>(Instance())->MessageProcess(hWnd, msg, wParam, lParam);
		}
	}
	
	/**
	 * \brief ��ʼ������
	 * \return �Ƿ��ʼ���ɹ�
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
	 * \brief ��ʼ��DirectX12
	 * \return �Ƿ��ʼ���ɹ�
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

		//����DXGI������ö���Կ�������SwapChain
		ReturnIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_DXGIFactory)));

		//������ʾ�豸(���豸��Ӧһ����ʾ�������������Կ�������nullptr����ʹ��Ĭ�ϵ���ʾ������)
		const auto result = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_D3D12Device));
		if (FAILED(result))
		{//Ĭ����ʾ��������Ӧ���豸����ʧ��
			ComPtr<IDXGIAdapter> pWarpAdapter;
			ReturnIfFailed(m_DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));
			ReturnIfFailed(D3D12CreateDevice(pWarpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_D3D12Device)))
		}

		//����Χ��
		ReturnIfFailed(m_D3D12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)))

		//�����������/����������������б�
		D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
		commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		ReturnIfFailed(m_D3D12Device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_CommandQueue)));
		ReturnIfFailed(m_D3D12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator)));
		ReturnIfFailed(m_D3D12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_CommandList)))
		m_CommandList->Close();

		//����������
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

		//����RTV�������Ѻ�RTV������
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		heapDesc.NumDescriptors = SwapChainBufferCount;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NodeMask = 0;
		ReturnIfFailed(m_D3D12Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_RtvDescriptorHeap.GetAddressOf())))

		//�������/�ü�����������������
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		heapDesc.NumDescriptors = 1;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NodeMask = 0;
		ReturnIfFailed(m_D3D12Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_DsvDescriptorHeap.GetAddressOf())))

		//��ѯ��������С
		m_RtvDescriptorSize = m_D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		m_DsvDescriptorSize = m_D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		m_CbvDescriptorSize = m_D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		
		return true;
	}

	/**
	 * \brief ��Ϣ����
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
	 * \brief ���ڱ仯�Ļص�����
	 */
	void D3D12Application::OnResize()
	{

	}
}

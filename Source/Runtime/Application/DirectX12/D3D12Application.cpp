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

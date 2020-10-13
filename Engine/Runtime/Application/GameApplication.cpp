#include "Application/GameApplication.h"

#include "Camera/CameraManager.h"
#include "Graphics/DirectX12/D3D12GraphicsManager.h"
#include "Graphics/ShaderManager.h"
#include "Input/Input.h"

namespace SaplingEngine
{
	GameApplication* GameApplication::s_Instance = nullptr;

	GameApplication::GameApplication()
	{
		s_Instance = this;
	}

	/**
	 * \brief 初始化程序配置
	 * \return 是否初始化成功
	 */
	bool GameApplication::InitializeConfig()
	{
		//加载XML配置
		XmlDocumentFile documentFile("Resources/Configs/ApplicationConfig.xml");
		m_ConfigXmlDocument.parse<0>(documentFile.data());

		//读取root节点
		auto* pRoot = m_ConfigXmlDocument.first_node();

		//读取窗口宽度和高度
		auto* pWindowWidthNode = pRoot->first_node("windowWidth");
		auto* pWindowHeightNode = pRoot->first_node("windowHeight");
		m_Width = std::stoi(pWindowWidthNode->value());
		m_Height = std::stoi(pWindowHeightNode->value());

		//读取启动场景配置
		auto* pSceneNode = pRoot->first_node("startScene");
		
		return true;
	}

	/**
	 * \brief 初始化App
	 * \param hInstance app句柄
	 * \return 是否初始化成功
	 */
	bool GameApplication::InitializeApplication(HINSTANCE hInstance)
	{
		m_AppInstance = hInstance;

		//初始化窗口
		auto result = InitializeWindow();
		if (!result)
		{
			//窗口初始化失败
			return false;
		}

		//初始化Graphics
		result = D3D12GraphicsManager::Instance()->InitializeGraphics(m_MainWindow, m_Width, m_Height);
		if (!result)
		{
			//Graphics初始化失败
			return false;
		}

		//显示并更新窗口
		ShowWindow(m_MainWindow, SW_SHOW);
		UpdateWindow(m_MainWindow);
		
		return true;
	}

	/**
	 * \brief 运行
	 */
	void GameApplication::Run()
	{
		Time::Initialize();
		
		MSG msg = {};
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{//处理消息
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				Time::Tick();

				//逻辑更新
				Update();

				//渲染
				D3D12GraphicsManager::Instance()->Render();
			}
		}
	}

	/**
	 * \brief 销毁
	 */
	void GameApplication::Destroy()
	{
		CameraManager::Release();
		Input::Release();
		ShaderManager::Release();
		D3D12GraphicsManager::Release();
	}

	/**
	 * \brief 初始化窗口
	 * \return 是否初始化成功
	 */
	bool GameApplication::InitializeWindow()
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

		RECT rect = { 0, 0, static_cast<long>(m_Width), static_cast<long>(m_Height) };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
		const int width = rect.right - rect.left;
		const int height = rect.bottom - rect.top;

		m_MainWindow = CreateWindow(L"MainWnd", L"Sapling", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, m_AppInstance, nullptr);
		if (!m_MainWindow)
		{
			MessageBox(nullptr, L"CreateWindow Failed.", nullptr, 0);
			return false;
		}
		
		return true;
	}

	/**
	 * \brief 更新
	 */
	void GameApplication::Update()
	{
		
	}

	/**
	 * \brief 消息处理回调函数
	 */
	LRESULT GameApplication::MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return Instance()->MessageProcess(hWnd, msg, wParam, lParam);
	}

	/**
	 * \brief 消息处理
	 */
	LRESULT GameApplication::MessageProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Input::Instance()->Reset();
		
		switch (msg)
		{
		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				m_IsActive = false;
				Time::Stop();
			}
			else
			{
				m_IsActive = true;
				Time::Start();
			}
			return 0;
			
		case WM_SIZE:
			m_Width = LOWORD(lParam);
			m_Height = HIWORD(lParam);
			if (wParam == SIZE_MINIMIZED)
			{
				m_IsActive = false;
				m_IsMinimized = true;
				m_IsMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_IsActive = true;
				m_IsMinimized = false;
				m_IsMaximized = true;
				D3D12GraphicsManager::Instance()->OnResize(m_Width, m_Height);
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (m_IsMinimized)
				{
					m_IsActive = true;
					m_IsMinimized = false;
					D3D12GraphicsManager::Instance()->OnResize(m_Width, m_Height);
				}
				else if (m_IsMaximized)
				{
					m_IsActive = true;
					m_IsMaximized = false;
					D3D12GraphicsManager::Instance()->OnResize(m_Width, m_Height);
				}
				else if (m_IsResizing)
				{
				}
				else
				{
					D3D12GraphicsManager::Instance()->OnResize(m_Width, m_Height);
				}
			}
			return 0;
		
		case WM_ENTERSIZEMOVE:
			m_IsActive = false;
			m_IsResizing = true;
			Time::Stop();
			return 0;
		
		case WM_EXITSIZEMOVE:
			m_IsActive = true;
			m_IsResizing = false;
			Time::Start();
			D3D12GraphicsManager::Instance()->OnResize(m_Width, m_Height);
			return 0;

		case WM_LBUTTONDOWN:
			Input::Instance()->SetMouseButton(EMouseButtonState::LeftMouseButtonDown, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
			
		case WM_RBUTTONDOWN:
			Input::Instance()->SetMouseButton(EMouseButtonState::RightMouseButtonDown, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;

		case WM_LBUTTONUP:
			Input::Instance()->SetMouseButton(EMouseButtonState::LeftMouseButtonUp, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
			
		case WM_RBUTTONUP:
			Input::Instance()->SetMouseButton(EMouseButtonState::RightMouseButtonUp, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;

		case WM_MOUSEMOVE:
			Input::Instance()->SetMouseButton(EMouseButtonState::MouseButtonMove, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;

		case WM_KEYDOWN:
			
			return 0;
			
		case WM_KEYUP:
			if (wParam == VK_ESCAPE)
			{
				PostQuitMessage(0);
			}
			return 0;
			
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}
}

#include "GameApplication.h"
#include "GameSetting.h"

#include "Camera/CameraManager.h"
#include "Input/Input.h"
#include "Render/Graphics/ShaderManager.h"
#include "Render/RenderPipeline/RenderPipeline.h"
#include "Resource/ResourceManager.h"
#include "Scene/SceneManager.h"

namespace SaplingEngine
{
	GameApplication* GameApplication::s_Instance = nullptr;

	GameApplication::GameApplication()
	{
		s_Instance = this;
	}

	/**
	 * \brief 初始化App
	 * \param hInstance app句柄
	 * \return 是否初始化成功
	 */
	bool GameApplication::Initialize(HINSTANCE hInstance)
	{
		m_AppInstance = hInstance;

		//初始化资源管理器
		ResourceManager::Instance()->Initialize();
		
		//初始化窗口
		if (InitializeWindow())
		{
			//渲染管线开始初始化
			RenderPipeline::Instance()->BeginInitialize(m_MainWindow);
			
			//初始化Shader
			ShaderManager::Instance()->Initialize();
			
			//初始化场景
			SceneManager::Instance()->Initialize();

			//渲染管线结束初始化
			RenderPipeline::Instance()->EndInitialize(m_MainWindow);
			
			//显示并更新窗口
			ShowWindow(m_MainWindow, SW_SHOW);
			UpdateWindow(m_MainWindow);
			
			return true;
		}

		return false;
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
				RenderPipeline::Instance()->Render();
			}
		}
	}

	/**
	 * \brief 销毁
	 */
	void GameApplication::Destroy()
	{
		CameraManager::Instance()->Destroy();
		Input::Instance()->Destroy();
		SceneManager::Instance()->Destroy();
		ShaderManager::Instance()->Destroy();
		RenderPipeline::Instance()->Destroy();
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

		RECT rect = { 0, 0, static_cast<long>(GameSetting::Instance()->ScreenWidth()), static_cast<long>(GameSetting::Instance()->ScreenHeight()) };
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
		SceneManager::Instance()->Update();
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
			GameSetting::Instance()->SetScreenSize(LOWORD(lParam), HIWORD(lParam));
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
				CameraManager::Instance()->OnWindowResize();
				RenderPipeline::Instance()->OnWindowResize();
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (m_IsMinimized)
				{
					m_IsActive = true;
					m_IsMinimized = false;
					CameraManager::Instance()->OnWindowResize();
					RenderPipeline::Instance()->OnWindowResize();
				}
				else if (m_IsMaximized)
				{
					m_IsActive = true;
					m_IsMaximized = false;
					CameraManager::Instance()->OnWindowResize();
					RenderPipeline::Instance()->OnWindowResize();
				}
				else if (m_IsResizing)
				{
				}
				else
				{
					CameraManager::Instance()->OnWindowResize();
					RenderPipeline::Instance()->OnWindowResize();
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
			CameraManager::Instance()->OnWindowResize();
			RenderPipeline::Instance()->OnWindowResize();
			return 0;

		case WM_MOUSEWHEEL:
			Input::Instance()->SetMouseButton(EMouseButtonState::MouseWheel, static_cast<short>(HIWORD(wParam)), 0);
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

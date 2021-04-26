#include "GameApplication.h"
#include "GameSetting.h"

#include "Input/Input.h"
#include "Render/Graphics/ShaderManager.h"
#include "Resource/ResourceManager.h"
#include "Scene/SceneManager.h"

namespace SaplingEngine
{
	//静态成员初始化
	HINSTANCE	GameApplication::appInstance = nullptr;
	HWND		GameApplication::windowHwnd = nullptr;
	bool		GameApplication::isActive = true;
	bool		GameApplication::isMinimized = false;
	bool		GameApplication::isMaximized = false;
	bool		GameApplication::isResizing = false;
	bool		GameApplication::isFullscreen = false;
	
	/**
	 * \brief	初始化App
	 * \param	hInstance		app句柄
	 * \return	是否初始化成功
	 */
	bool GameApplication::Initialize(HINSTANCE hInstance)
	{
		appInstance = hInstance;

		//初始化窗口
		if (!InitializeWindow())
		{
			return false;
		}
		
		//初始化资源管理器
		ResourceManager::Initialize();

		//渲染管线开始初始化
		RenderPipeline::BeginInitialize(windowHwnd);

		//初始化Shader
		ShaderManager::Instance()->Initialize();

		//初始化场景
		SceneManager::Initialize();

		//渲染管线结束初始化
		RenderPipeline::EndInitialize(windowHwnd);

		//显示并更新窗口
		ShowWindow(windowHwnd, SW_SHOW);
		UpdateWindow(windowHwnd);

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
				RenderPipeline::Render();
			}
		}
	}

	/**
	 * \brief 销毁
	 */
	void GameApplication::Destroy()
	{
		CameraManager::Destroy();
		Input::Destroy();
		SceneManager::Destroy();
		ShaderManager::Instance()->Destroy();
		RenderPipeline::Destroy();
	}

	/**
	 * \brief 初始化窗口
	 * \return 是否初始化成功
	 */
	bool GameApplication::InitializeWindow()
	{
		WNDCLASS wc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = MessageProcess;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = appInstance;
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

		RECT rect = { 0, 0, static_cast<long>(GameSetting::ScreenWidth()), static_cast<long>(GameSetting::ScreenHeight()) };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
		const int width = rect.right - rect.left;
		const int height = rect.bottom - rect.top;

		windowHwnd = CreateWindow(L"MainWnd", L"Sapling", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, appInstance, nullptr);
		if (!windowHwnd)
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
		SceneManager::Update();
	}

	/**
	 * \brief 消息处理
	 */
	LRESULT GameApplication::MessageProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Input::Reset();
		
		switch (msg)
		{
		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				isActive = false;
				Time::Stop();
			}
			else
			{
				isActive = true;
				Time::Start();
			}
			return 0;
			
		case WM_SIZE:
			if (wParam == SIZE_MINIMIZED)
			{
				isActive = false;
				isMinimized = true;
				isMaximized = false;
				return 0;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				isActive = true;
				isMinimized = false;
				isMaximized = true;
				
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (isMinimized)
				{
					isActive = true;
					isMinimized = false;
				}
				else if (isMaximized)
				{
					isActive = true;
					isMaximized = false;
				}
				else if (isResizing)
				{
					return 0;
				}
				else
				{
				}
			}
			GameSetting::SetScreenSize(LOWORD(lParam), HIWORD(lParam));
			return 0;
		
		case WM_ENTERSIZEMOVE:
			isActive = false;
			isResizing = true;
			Time::Stop();
			return 0;
		
		case WM_EXITSIZEMOVE:
			isActive = true;
			isResizing = false;
			Time::Start();
			return 0;

		case WM_MOUSEWHEEL:
			Input::SetMouseButton(EMouseButtonState::MouseWheel, static_cast<short>(HIWORD(wParam)), 0);
			return 0;
			
		case WM_LBUTTONDOWN:
			Input::SetMouseButton(EMouseButtonState::LeftMouseButtonDown, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
			
		case WM_RBUTTONDOWN:
			Input::SetMouseButton(EMouseButtonState::RightMouseButtonDown, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;

		case WM_LBUTTONUP:
			Input::SetMouseButton(EMouseButtonState::LeftMouseButtonUp, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
			
		case WM_RBUTTONUP:
			Input::SetMouseButton(EMouseButtonState::RightMouseButtonUp, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;

		case WM_MOUSEMOVE:
			Input::SetMouseButton(EMouseButtonState::MouseButtonMove, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
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

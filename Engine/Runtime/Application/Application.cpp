#include "Application/Application.h"
#include "Application/Setting.h"
#include "Input/Input.h"
#include "Render/Graphics/MaterialManager.h"
#include "Render/Graphics/ShaderManager.h"
#include "Render/Graphics/TextureManager.h"
#include "Resource/ResourceManager.h"
#include "Scene/SceneManager.h"

namespace SaplingEngine
{
	//静态成员初始化
	HINSTANCE	Application::appInstance = nullptr;
	HWND		Application::windowHwnd = nullptr;
	bool		Application::isActive = true;
	bool		Application::isMinimized = false;
	bool		Application::isMaximized = false;
	bool		Application::isResizing = false;
	bool		Application::isFullscreen = false;
	
	/**
	 * \brief	初始化App
	 * \param	hInstance		app句柄
	 * \return	是否初始化成功
	 */
	bool Application::Initialize(HINSTANCE hInstance)
	{
		appInstance = hInstance;

		//初始化窗口
		if (!InitializeWindow())
		{
			return false;
		}
		
		//初始化资源管理器
		ResourceManager::Initialize();

		//初始化Shader
		ShaderManager::Initialize();

		//贴图初始化
		TextureManager::Initialize();

		//材质初始化
		MaterialManager::Initialize();

		//初始化场景
		SceneManager::Initialize();

		//时间初始化
		Time::Initialize();

		//渲染管线开始初始化
		RenderPipeline::Initialize(windowHwnd);

		//显示并更新窗口
		ShowWindow(windowHwnd, SW_SHOW);
		UpdateWindow(windowHwnd);

		return true;
	}

	/**
	 * \brief 运行
	 */
	void Application::Run()
	{
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
				//时间更新
				Time::Tick();

				//Input刷新
				Input::Refresh();

				//逻辑更新
				Update();

				//Input重置
				Input::Reset();

				//渲染
				RenderPipeline::Render();
			}
		}
	}

	/**
	 * \brief 销毁
	 */
	void Application::Destroy()
	{
		CameraManager::Destroy();
		Input::Destroy();
		MaterialManager::Destroy();
		SceneManager::Destroy();
		ShaderManager::Destroy();
		RenderPipeline::Destroy();
	}

	/**
	 * \brief 消息处理
	 */
	LRESULT Application::MessageProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
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
						Setting::SetScreenSize(LOWORD(lParam), HIWORD(lParam));
						return 0;
					}
					else
					{
					}
				}
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
				Input::SetKeyState(EKeyState::KeyDown, static_cast<EKeyCode>(wParam));
				return 0;

			case WM_KEYUP:
				if (wParam == VK_ESCAPE)
				{
					PostQuitMessage(0);
				}
				else
				{
					Input::SetKeyState(EKeyState::KeyUp, static_cast<EKeyCode>(wParam));
				}
				return 0;

			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;

			default:
				return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}

	/**
	 * \brief 初始化窗口
	 * \return 是否初始化成功
	 */
	bool Application::InitializeWindow()
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

		RECT rect = { 0, 0, static_cast<long>(Setting::ScreenWidth()), static_cast<long>(Setting::ScreenHeight()) };
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
	void Application::Update()
	{
		GameObject::UpdateAll();
	}
}

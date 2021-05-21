#include "Editor/Application/EditorApplication.h"
#include "Editor/Input/EditorInputSystem.h"

using namespace SaplingEngine;

namespace SaplingEditor
{
	/**
	 * \brief	初始化App
	 * \param	hInstance		app句柄
	 * \param	hwnd			窗口句柄
	 * \return	是否初始化成功
	 */
	bool EditorApplication::Initialize(HINSTANCE hInstance, HWND hwnd)
	{
		Application::appInstance = hInstance;
		Application::windowHwnd = hwnd;

		//初始化资源管理器
		ResourceManager::Initialize();

		//渲染管线开始初始化
		RenderPipeline::BeginInitialize(Application::windowHwnd);

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

		//渲染管线结束初始化
		RenderPipeline::EndInitialize(Application::windowHwnd);

		//显示并更新窗口
		ShowWindow(Application::windowHwnd, SW_SHOW);
		UpdateWindow(Application::windowHwnd);

		return true;
	}
	
	/**
	 * \brief	运行
	 */
	void EditorApplication::Run()
	{
		//游戏更新
		Update();
	}
	
	/**
	 * \brief	销毁
	 */
	void EditorApplication::Destroy()
	{
		Application::Destroy();
	}
	
	/**
	 * \brief 消息处理
	 */
	LRESULT EditorApplication::MessageProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			case WM_ACTIVATE:
				if (LOWORD(wParam) == WA_INACTIVE)
				{
					Application::isActive = false;
					Time::Stop();
				}
				else
				{
					Application::isActive = true;
					Time::Start();
				}
				return 0;

			case WM_ENTERSIZEMOVE:
				Application::isActive = false;
				Application::isResizing = true;
				Time::Stop();
				return 0;

			case WM_EXITSIZEMOVE:
				Application::isActive = true;
				Application::isResizing = false;
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
	 * \brief	更新
	 */
	void EditorApplication::Update()
	{
		//时间更新
		Time::Tick();

		//Input刷新
		Input::Refresh();

		//编辑器输入系统更新
		EditorInputSystem::Update();

		//逻辑更新
		Application::Update();

		//Input重置
		Input::Reset();

		//渲染
		RenderPipeline::Render();
	}
}
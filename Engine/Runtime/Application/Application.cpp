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
	//��̬��Ա��ʼ��
	HINSTANCE	Application::appInstance = nullptr;
	HWND		Application::windowHwnd = nullptr;
	bool		Application::isActive = true;
	bool		Application::isMinimized = false;
	bool		Application::isMaximized = false;
	bool		Application::isResizing = false;
	bool		Application::isFullscreen = false;
	
	/**
	 * \brief	��ʼ��App
	 * \param	hInstance		app���
	 * \return	�Ƿ��ʼ���ɹ�
	 */
	bool Application::Initialize(HINSTANCE hInstance)
	{
		appInstance = hInstance;

		//��ʼ������
		if (!InitializeWindow())
		{
			return false;
		}
		
		//��ʼ����Դ������
		ResourceManager::Initialize();

		//��ʼ��Shader
		ShaderManager::Initialize();

		//��ͼ��ʼ��
		TextureManager::Initialize();

		//���ʳ�ʼ��
		MaterialManager::Initialize();

		//��ʼ������
		SceneManager::Initialize();

		//ʱ���ʼ��
		Time::Initialize();

		//��Ⱦ���߿�ʼ��ʼ��
		RenderPipeline::Initialize(windowHwnd);

		//��ʾ�����´���
		ShowWindow(windowHwnd, SW_SHOW);
		UpdateWindow(windowHwnd);

		return true;
	}

	/**
	 * \brief ����
	 */
	void Application::Run()
	{
		MSG msg = {};
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{//������Ϣ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//ʱ�����
				Time::Tick();

				//Inputˢ��
				Input::Refresh();

				//�߼�����
				Update();

				//Input����
				Input::Reset();

				//��Ⱦ
				RenderPipeline::Render();
			}
		}
	}

	/**
	 * \brief ����
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
	 * \brief ��Ϣ����
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
	 * \brief ��ʼ������
	 * \return �Ƿ��ʼ���ɹ�
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
	 * \brief ����
	 */
	void Application::Update()
	{
		GameObject::UpdateAll();
	}
}

#include "Editor/Application/EditorApplication.h"
#include "Editor/Input/EditorInputSystem.h"

using namespace SaplingEngine;

namespace SaplingEditor
{
	/**
	 * \brief	��ʼ��App
	 * \param	hInstance		app���
	 * \param	hwnd			���ھ��
	 * \return	�Ƿ��ʼ���ɹ�
	 */
	bool EditorApplication::Initialize(HINSTANCE hInstance, HWND hwnd)
	{
		Application::appInstance = hInstance;
		Application::windowHwnd = hwnd;

		//��ʼ����Դ������
		ResourceManager::Initialize();

		//��Ⱦ���߿�ʼ��ʼ��
		RenderPipeline::BeginInitialize(Application::windowHwnd);

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

		//��Ⱦ���߽�����ʼ��
		RenderPipeline::EndInitialize(Application::windowHwnd);

		//��ʾ�����´���
		ShowWindow(Application::windowHwnd, SW_SHOW);
		UpdateWindow(Application::windowHwnd);

		return true;
	}
	
	/**
	 * \brief	����
	 */
	void EditorApplication::Run()
	{
		//��Ϸ����
		Update();
	}
	
	/**
	 * \brief	����
	 */
	void EditorApplication::Destroy()
	{
		Application::Destroy();
	}
	
	/**
	 * \brief ��Ϣ����
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
	 * \brief	����
	 */
	void EditorApplication::Update()
	{
		//ʱ�����
		Time::Tick();

		//Inputˢ��
		Input::Refresh();

		//�༭������ϵͳ����
		EditorInputSystem::Update();

		//�߼�����
		Application::Update();

		//Input����
		Input::Reset();

		//��Ⱦ
		RenderPipeline::Render();
	}
}
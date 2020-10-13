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
	 * \brief ��ʼ����������
	 * \return �Ƿ��ʼ���ɹ�
	 */
	bool GameApplication::InitializeConfig()
	{
		//����XML����
		XmlDocumentFile documentFile("Resources/Configs/ApplicationConfig.xml");
		m_ConfigXmlDocument.parse<0>(documentFile.data());

		//��ȡroot�ڵ�
		auto* pRoot = m_ConfigXmlDocument.first_node();

		//��ȡ���ڿ�Ⱥ͸߶�
		auto* pWindowWidthNode = pRoot->first_node("windowWidth");
		auto* pWindowHeightNode = pRoot->first_node("windowHeight");
		m_Width = std::stoi(pWindowWidthNode->value());
		m_Height = std::stoi(pWindowHeightNode->value());

		//��ȡ������������
		auto* pSceneNode = pRoot->first_node("startScene");
		
		return true;
	}

	/**
	 * \brief ��ʼ��App
	 * \param hInstance app���
	 * \return �Ƿ��ʼ���ɹ�
	 */
	bool GameApplication::InitializeApplication(HINSTANCE hInstance)
	{
		m_AppInstance = hInstance;

		//��ʼ������
		auto result = InitializeWindow();
		if (!result)
		{
			//���ڳ�ʼ��ʧ��
			return false;
		}

		//��ʼ��Graphics
		result = D3D12GraphicsManager::Instance()->InitializeGraphics(m_MainWindow, m_Width, m_Height);
		if (!result)
		{
			//Graphics��ʼ��ʧ��
			return false;
		}

		//��ʾ�����´���
		ShowWindow(m_MainWindow, SW_SHOW);
		UpdateWindow(m_MainWindow);
		
		return true;
	}

	/**
	 * \brief ����
	 */
	void GameApplication::Run()
	{
		Time::Initialize();
		
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
				Time::Tick();

				//�߼�����
				Update();

				//��Ⱦ
				D3D12GraphicsManager::Instance()->Render();
			}
		}
	}

	/**
	 * \brief ����
	 */
	void GameApplication::Destroy()
	{
		CameraManager::Release();
		Input::Release();
		ShaderManager::Release();
		D3D12GraphicsManager::Release();
	}

	/**
	 * \brief ��ʼ������
	 * \return �Ƿ��ʼ���ɹ�
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
	 * \brief ����
	 */
	void GameApplication::Update()
	{
		
	}

	/**
	 * \brief ��Ϣ����ص�����
	 */
	LRESULT GameApplication::MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return Instance()->MessageProcess(hWnd, msg, wParam, lParam);
	}

	/**
	 * \brief ��Ϣ����
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

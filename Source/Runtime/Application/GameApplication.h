#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class GameApplication
	{
	protected:
		GameApplication();
		virtual ~GameApplication();

	public:
		GameApplication(const GameApplication&) = delete;
		GameApplication(GameApplication&&) = delete;
		GameApplication& operator=(const GameApplication&) = delete;
		GameApplication& operator=(GameApplication&&) = delete;

		/*
		 * ����
		 */
		static GameApplication* Instance()
		{
			return s_Instance;
		}

		static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			return s_Instance->MessageProcess(hwnd, msg, wParam, lParam);
		}

		/*
		 * ��ʼ��
		 */
		virtual bool Initialize(HINSTANCE hInstance);

		/*
		 * ����
		 */
		int Run();

	protected:
		/*
		 * ��ʼ������
		 */
		virtual bool InitializeConfig();

		/*
		 * ��ʼ������
		 */
		virtual bool InitializeWindow();

		/*
		 * ��Ϣ����
		 */
		virtual LRESULT MessageProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		/*
		 * ���ڱ仯�ص�
		 */
		virtual void OnResize();

		/*
		 * ����
		 */
		virtual void Update() = 0;

		/*
		 * ����
		 */
		virtual void Render() = 0;

	protected:
		static GameApplication* s_Instance;

		/*
		 * Application ���
		 */
		HINSTANCE m_AppInstance = nullptr;

		/*
		 * ���ھ��
		 */
		HWND m_MainWindow = nullptr;

		/*
		 * ���ڿ�͸�
		 */
		int32_t m_Width = 0;
		int32_t m_Height = 0;
	};
}

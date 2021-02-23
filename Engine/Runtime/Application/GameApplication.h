#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class GameApplication final
	{
	public:
		/**
		 * \brief ��ʼ��App
		 * \param hInstance app���
		 * \return �Ƿ��ʼ���ɹ�
		 */
		static bool Initialize(HINSTANCE hInstance);

		/*
		 * ����
		 */
		static void Run();

		/**
		 * \brief ����
		 */
		static void Destroy();

	private:
		/**
		 * \brief ��ʼ������
		 * \return �Ƿ��ʼ���ɹ�
		 */
		static bool InitializeWindow();

		/**
		 * \brief ����
		 */
		static void Update();

		/*
		 * ��Ϣ����ص�
		 */
		static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		/*
		 * ��Ϣ����
		 */
		static LRESULT MessageProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		/**
		 * \brief Application ���
		 */
		static HINSTANCE m_AppInstance;

		/**
		 * \brief ���ھ��
		 */
		static HWND m_WindowHwnd;

		/**
		 * \brief �Ƿ��ڼ���״̬
		 */
		static bool m_IsActive;
		
		/**
		 * \brief �����Ƿ���С��
		 */
		static bool m_IsMinimized;
		
		/**
		 * \brief �����Ƿ����
		 */
		static bool m_IsMaximized;
		
		/**
		 * \brief �����Ƿ����ڸı��С
		 */
		static bool m_IsResizing;
		
		/**
		 * \brief �Ƿ�ȫ��״̬
		 */
		static bool m_IsFullscreen;
	};
}

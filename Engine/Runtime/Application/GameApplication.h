#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class GameApplication
	{
	public:
		GameApplication();
		~GameApplication() = default;

		GameApplication(const GameApplication&) = delete;
		GameApplication(GameApplication&&) = delete;
		GameApplication& operator=(const GameApplication&) = delete;
		GameApplication& operator=(GameApplication&&) = delete;

		/**
		 * \brief ����
		 * \return ����
		 */
		static GameApplication* Instance() { return s_Instance; }

		/**
		 * \brief ��ʼ��App
		 * \param hInstance app���
		 * \return �Ƿ��ʼ���ɹ�
		 */
		bool InitializeApplication(HINSTANCE hInstance);

		/*
		 * ����
		 */
		void Run();

		/**
		 * \brief ����
		 */
		void Destroy();

	private:
		/**
		 * \brief ��ʼ������
		 * \return �Ƿ��ʼ���ɹ�
		 */
		bool InitializeWindow();

		/**
		 * \brief ����
		 */
		void Update();

		/*
		 * ��Ϣ����ص�
		 */
		static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		/*
		 * ��Ϣ����
		 */
		LRESULT MessageProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		static GameApplication* s_Instance;

		/**
		 * \brief Application ���
		 */
		HINSTANCE m_AppInstance = nullptr;

		/**
		 * \brief ���ھ��
		 */
		HWND m_MainWindow = nullptr;

		/**
		 * \brief �Ƿ��ڼ���״̬
		 */
		bool m_IsActive = false;
		
		/**
		 * \brief �����Ƿ���С��
		 */
		bool m_IsMinimized = false;
		
		/**
		 * \brief �����Ƿ����
		 */
		bool m_IsMaximized = false;
		
		/**
		 * \brief �����Ƿ����ڸı��С
		 */
		bool m_IsResizing = false;
		
		/**
		 * \brief �Ƿ�ȫ��״̬
		 */
		bool m_IsFullscreen = false;

		/**
		 * \brief ��������
		 */
		XmlDocument m_ConfigXmlDocument;
	};
}

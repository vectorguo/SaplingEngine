#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class GameApplication
	{
	protected:
		GameApplication();

	public:
		virtual ~GameApplication();

	public:
		GameApplication(const GameApplication&) = delete;
		GameApplication(GameApplication&&) = delete;
		GameApplication& operator=(const GameApplication&) = delete;
		GameApplication& operator=(GameApplication&&) = delete;

		/*
		 * ����
		 */
		static GameApplication* Instance() { return s_Instance; }

		/*
		 * ��ʼ��
		 */
		virtual bool InitializeApplicationConfig() = 0;
		virtual bool InitializeSceneConfig() = 0;
		virtual bool InitializeApplication(HINSTANCE hInstance);

		/*
		 * ����
		 */
		void Run();

		/**
		 * \brief ����
		 */
		virtual void Destroy();

	protected:
		/**
		 * \brief ��ʼ������
		 * \return �Ƿ��ʼ���ɹ�
		 */
		virtual bool InitializeWindow();

		/**
		 * \brief ��ʼ��ͼ������
		 * \return �Ƿ��ʼ���ɹ�
		 */
		virtual bool InitializeGraphics() = 0;
		
		/*
		 * ����
		 */
		virtual void Update() = 0;

		/*
		 * ����
		 */
		virtual void Render() = 0;

		/*
		 * ���ڱ仯�ص�
		 */
		virtual void OnResize() = 0;

	private:
		/*
		 * ��Ϣ����ص�
		 */
		static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		/*
		 * ��Ϣ����
		 */
		LRESULT MessageProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	protected:
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
		 * \brief ���ڿ��
		 */
		uint32_t m_Width = 0;
		
		/**
		 * \brief ���ڸ߶�
		 */
		uint32_t m_Height = 0;

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
	};
}

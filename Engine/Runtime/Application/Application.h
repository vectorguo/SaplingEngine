#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Application final
	{
	public:
		/**
		 * \brief	��ʼ��App
		 * \param	hInstance		app���
		 * \return	�Ƿ��ʼ���ɹ�
		 */
		static bool Initialize(HINSTANCE hInstance);

		/**
		 * \brief	��ʼ��App
		 * \param	hInstance		app���
		 * \param	hwnd			���ھ��
		 * \return	�Ƿ��ʼ���ɹ�
		 */
		static bool Initialize(HINSTANCE hInstance, HWND hwnd);

		/*
		 * \brief	����
		 */
		static void Run();

		/**
		 * \brief	����
		 */
		static void Destroy();

	private:
		/**
		 * \brief	��ʼ������
		 * \return	�Ƿ��ʼ���ɹ�
		 */
		static bool InitializeWindow();

		/**
		 * \brief	��ʼ������
		 * \param	hwnd			���ھ��
		 * \return	�Ƿ��ʼ���ɹ�
		 */
		static bool InitializeWindow(HWND hwnd);

		/**
		 * \brief	����
		 */
		static void Update();

		/**
		 * \brief	��Ϣ����
		 */
		static LRESULT CALLBACK MessageProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		/**
		 * \brief	Application���
		 */
		static HINSTANCE appInstance;

		/**
		 * \brief	���ھ��
		 */
		static HWND windowHwnd;

		/**
		 * \brief	�Ƿ��ڼ���״̬
		 */
		static bool isActive;
		
		/**
		 * \brief	�����Ƿ���С��
		 */
		static bool isMinimized;
		
		/**
		 * \brief	�����Ƿ����
		 */
		static bool isMaximized;
		
		/**
		 * \brief	�����Ƿ����ڸı��С
		 */
		static bool isResizing;
		
		/**
		 * \brief	�Ƿ�ȫ��״̬
		 */
		static bool isFullscreen;
	};
}

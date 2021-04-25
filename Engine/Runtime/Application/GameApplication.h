#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class GameApplication final
	{
	public:
		/**
		 * \brief	��ʼ��App
		 * \param	hInstance	app���
		 * \return	�Ƿ��ʼ���ɹ�
		 */
		static bool Initialize(HINSTANCE hInstance);

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
		 * \brief	����
		 */
		static void Update();

		/**
		 * \brief	��Ϣ����
		 */
		static LRESULT CALLBACK MessageProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		/**
		 * \brief	Application ���
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

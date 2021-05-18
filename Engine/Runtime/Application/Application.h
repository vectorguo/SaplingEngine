#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Application
	{
#if SAPLING_EDITOR
		friend class SaplingEditor::EditorApplication;
#endif

	public:
		/**
		 * \brief	��ʼ��App
		 * \param	hInstance		app���
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

		/**
		 * \brief	��Ϣ����
		 */
		static LRESULT CALLBACK MessageProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	protected:
		/**
		 * \brief	��ʼ������
		 * \return	�Ƿ��ʼ���ɹ�
		 */
		static bool InitializeWindow();

		/**
		 * \brief	����
		 */
		static void Update();

	protected:
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

#pragma once

#include "Editor/Utility/EditorUtility.h"

namespace SaplingEditor
{
	class EditorApplication
	{
	public:
		/**
		 * \brief	��ʼ��App
		 * \param	hInstance		app���
		 * \param	hwnd			���ھ��
		 * \return	�Ƿ��ʼ���ɹ�
		 */
		static bool Initialize(HINSTANCE hInstance, HWND hwnd);

		/**
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

	private:
		/**
		 * \brief	����
		 */
		static void Update();
	};
}
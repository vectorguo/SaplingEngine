#pragma once

#include "SaplingEnginePch.h"

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

	private:
		/**
		 * \brief	����
		 */
		static void Update();
	};
}
#pragma once

#include "Editor/Utility/EditorUtility.h"

namespace SaplingEditor
{
	class EditorApplication
	{
	public:
		/**
		 * \brief	初始化App
		 * \param	hInstance		app句柄
		 * \param	hwnd			窗口句柄
		 * \return	是否初始化成功
		 */
		static bool Initialize(HINSTANCE hInstance, HWND hwnd);

		/**
		 * \brief	运行
		 */
		static void Run();

		/**
		 * \brief	销毁
		 */
		static void Destroy();

		/**
		 * \brief	消息处理
		 */
		static LRESULT CALLBACK MessageProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		/**
		 * \brief	更新
		 */
		static void Update();
	};
}
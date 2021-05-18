#pragma once

#include "SaplingEnginePch.h"

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

	private:
		/**
		 * \brief	更新
		 */
		static void Update();
	};
}
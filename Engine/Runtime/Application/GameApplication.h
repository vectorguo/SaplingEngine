#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class GameApplication final
	{
	public:
		/**
		 * \brief	初始化App
		 * \param	hInstance	app句柄
		 * \return	是否初始化成功
		 */
		static bool Initialize(HINSTANCE hInstance);

		/*
		 * \brief	运行
		 */
		static void Run();

		/**
		 * \brief	销毁
		 */
		static void Destroy();

	private:
		/**
		 * \brief	初始化窗口
		 * \return	是否初始化成功
		 */
		static bool InitializeWindow();

		/**
		 * \brief	更新
		 */
		static void Update();

		/**
		 * \brief	消息处理
		 */
		static LRESULT CALLBACK MessageProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		/**
		 * \brief	Application 句柄
		 */
		static HINSTANCE appInstance;

		/**
		 * \brief	窗口句柄
		 */
		static HWND windowHwnd;

		/**
		 * \brief	是否处于激活状态
		 */
		static bool isActive;
		
		/**
		 * \brief	窗口是否最小化
		 */
		static bool isMinimized;
		
		/**
		 * \brief	窗口是否最大化
		 */
		static bool isMaximized;
		
		/**
		 * \brief	窗口是否正在改变大小
		 */
		static bool isResizing;
		
		/**
		 * \brief	是否全屏状态
		 */
		static bool isFullscreen;
	};
}

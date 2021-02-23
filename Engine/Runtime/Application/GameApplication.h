#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class GameApplication final
	{
	public:
		/**
		 * \brief 初始化App
		 * \param hInstance app句柄
		 * \return 是否初始化成功
		 */
		static bool Initialize(HINSTANCE hInstance);

		/*
		 * 运行
		 */
		static void Run();

		/**
		 * \brief 销毁
		 */
		static void Destroy();

	private:
		/**
		 * \brief 初始化窗口
		 * \return 是否初始化成功
		 */
		static bool InitializeWindow();

		/**
		 * \brief 更新
		 */
		static void Update();

		/*
		 * 消息处理回调
		 */
		static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		/*
		 * 消息处理
		 */
		static LRESULT MessageProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		/**
		 * \brief Application 句柄
		 */
		static HINSTANCE m_AppInstance;

		/**
		 * \brief 窗口句柄
		 */
		static HWND m_WindowHwnd;

		/**
		 * \brief 是否处于激活状态
		 */
		static bool m_IsActive;
		
		/**
		 * \brief 窗口是否最小化
		 */
		static bool m_IsMinimized;
		
		/**
		 * \brief 窗口是否最大化
		 */
		static bool m_IsMaximized;
		
		/**
		 * \brief 窗口是否正在改变大小
		 */
		static bool m_IsResizing;
		
		/**
		 * \brief 是否全屏状态
		 */
		static bool m_IsFullscreen;
	};
}

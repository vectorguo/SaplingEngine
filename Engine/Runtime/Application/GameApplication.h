#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class GameApplication
	{
	public:
		GameApplication();
		~GameApplication() = default;

		GameApplication(const GameApplication&) = delete;
		GameApplication(GameApplication&&) = delete;
		GameApplication& operator=(const GameApplication&) = delete;
		GameApplication& operator=(GameApplication&&) = delete;

		/**
		 * \brief 单例
		 * \return 单例
		 */
		static GameApplication* Instance() { return s_Instance; }

		/**
		 * \brief 初始化App
		 * \param hInstance app句柄
		 * \return 是否初始化成功
		 */
		bool InitializeApplication(HINSTANCE hInstance);

		/*
		 * 运行
		 */
		void Run();

		/**
		 * \brief 销毁
		 */
		void Destroy();

	private:
		/**
		 * \brief 初始化窗口
		 * \return 是否初始化成功
		 */
		bool InitializeWindow();

		/**
		 * \brief 更新
		 */
		void Update();

		/*
		 * 消息处理回调
		 */
		static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		/*
		 * 消息处理
		 */
		LRESULT MessageProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		static GameApplication* s_Instance;

		/**
		 * \brief Application 句柄
		 */
		HINSTANCE m_AppInstance = nullptr;

		/**
		 * \brief 窗口句柄
		 */
		HWND m_MainWindow = nullptr;

		/**
		 * \brief 是否处于激活状态
		 */
		bool m_IsActive = false;
		
		/**
		 * \brief 窗口是否最小化
		 */
		bool m_IsMinimized = false;
		
		/**
		 * \brief 窗口是否最大化
		 */
		bool m_IsMaximized = false;
		
		/**
		 * \brief 窗口是否正在改变大小
		 */
		bool m_IsResizing = false;
		
		/**
		 * \brief 是否全屏状态
		 */
		bool m_IsFullscreen = false;

		/**
		 * \brief 程序配置
		 */
		XmlDocument m_ConfigXmlDocument;
	};
}

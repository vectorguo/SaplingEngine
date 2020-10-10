#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class GameApplication
	{
	protected:
		GameApplication();

	public:
		virtual ~GameApplication();

	public:
		GameApplication(const GameApplication&) = delete;
		GameApplication(GameApplication&&) = delete;
		GameApplication& operator=(const GameApplication&) = delete;
		GameApplication& operator=(GameApplication&&) = delete;

		/*
		 * 单例
		 */
		static GameApplication* Instance() { return s_Instance; }

		/*
		 * 初始化
		 */
		virtual bool InitializeApplicationConfig() = 0;
		virtual bool InitializeSceneConfig() = 0;
		virtual bool InitializeApplication(HINSTANCE hInstance);

		/*
		 * 运行
		 */
		void Run();

		/**
		 * \brief 销毁
		 */
		virtual void Destroy();

	protected:
		/**
		 * \brief 初始化窗口
		 * \return 是否初始化成功
		 */
		virtual bool InitializeWindow();

		/**
		 * \brief 初始化图形引擎
		 * \return 是否初始化成功
		 */
		virtual bool InitializeGraphics() = 0;
		
		/*
		 * 更新
		 */
		virtual void Update() = 0;

		/*
		 * 绘制
		 */
		virtual void Render() = 0;

		/*
		 * 窗口变化回调
		 */
		virtual void OnResize() = 0;

	private:
		/*
		 * 消息处理回调
		 */
		static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		/*
		 * 消息处理
		 */
		LRESULT MessageProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	protected:
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
		 * \brief 窗口宽度
		 */
		uint32_t m_Width = 0;
		
		/**
		 * \brief 窗口高度
		 */
		uint32_t m_Height = 0;

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
	};
}

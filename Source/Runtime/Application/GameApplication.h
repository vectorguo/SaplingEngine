#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class GameApplication
	{
	protected:
		GameApplication();
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
		virtual bool InitializeConfig() = 0;
		virtual bool InitializeApplication(HINSTANCE hInstance) = 0;

		/*
		 * 运行
		 */
		int Run();

	protected:
		/*
		 * 更新
		 */
		virtual void Update() = 0;

		/*
		 * 绘制
		 */
		virtual void Render() = 0;

	protected:
		static GameApplication* s_Instance;

		/*
		 * Application 句柄
		 */
		HINSTANCE m_AppInstance = nullptr;

		/*
		 * 窗口句柄
		 */
		HWND m_MainWindow = nullptr;

		/*
		 * 窗口宽和高
		 */
		int32_t m_Width = 0;
		int32_t m_Height = 0;
	};
}

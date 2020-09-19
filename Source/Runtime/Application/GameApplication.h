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
		 * ����
		 */
		static GameApplication* Instance() { return s_Instance; }

		/*
		 * ��ʼ��
		 */
		virtual bool InitializeConfig() = 0;
		virtual bool InitializeApplication(HINSTANCE hInstance) = 0;

		/*
		 * ����
		 */
		int Run();

	protected:
		/*
		 * ����
		 */
		virtual void Update() = 0;

		/*
		 * ����
		 */
		virtual void Render() = 0;

	protected:
		static GameApplication* s_Instance;

		/*
		 * Application ���
		 */
		HINSTANCE m_AppInstance = nullptr;

		/*
		 * ���ھ��
		 */
		HWND m_MainWindow = nullptr;

		/*
		 * ���ڿ�͸�
		 */
		int32_t m_Width = 0;
		int32_t m_Height = 0;
	};
}

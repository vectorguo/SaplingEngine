#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class GraphicsManager
	{
	public:
		GraphicsManager();
		virtual ~GraphicsManager() = default;

		GraphicsManager(const GraphicsManager&) = delete;
		GraphicsManager(GraphicsManager&&) = delete;
		GraphicsManager& operator=(const GraphicsManager&) = delete;
		GraphicsManager& operator=(GraphicsManager&&) = delete;

		/**
		 * \brief 获取单例
		 * \return 单例
		 */
		static GraphicsManager* Instance()
		{
			return m_Instance;
		}

		/**
		 * \brief 开始初始化
		 */
		virtual void BeginInitialize(HWND hWnd, uint32_t width, uint32_t height)
		{
			
		}

		/**
		 * \brief 结束初始化
		 */
		virtual void EndInitialize(HWND hWnd, uint32_t width, uint32_t height)
		{
			
		}
		
		/**
		 * \brief 销毁
		 */
		virtual void OnWindowResize(uint32_t width, uint32_t height)
		{

		}
		
		/**
		 * \brief 销毁
		 */
		virtual void Destroy()
		{

		}

	private:
		/**
		 * \brief 单例
		 */
		static GraphicsManager* m_Instance;
	};
}
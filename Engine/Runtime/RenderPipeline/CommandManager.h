#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Renderer;
	
	class CommandManager
	{
	public:
		CommandManager();
		virtual ~CommandManager() = default;

		CommandManager(const CommandManager&) = delete;
		CommandManager(CommandManager&&) = delete;
		CommandManager& operator=(const CommandManager&) = delete;
		CommandManager& operator=(CommandManager&&) = delete;

		/**
		 * \brief 获取单例
		 * \return 单例
		 */
		static CommandManager* Instance()
		{
			return m_Instance;
		}

		/**
		 * \brief 开始初始化
		 */
		virtual void BeginInitialize() {}

		/**
		 * \brief 结束初始化
		 */
		virtual void EndInitialize() {}

		/**
		 * \brief 执行渲染前的准备工作
		 */
		virtual void PreRender() {}

		/**
		 * \brief 执行渲染后的清理工作
		 */
		virtual void PostRender() {}

		/**
		 * \brief 执行绘制前的准备工作
		 */
		virtual void PreDraw(bool clearColor, bool clearDepth, const Color& color) {}

		/**
		 * \brief 执行绘制后的清理工作
		 */
		virtual void PostDraw() {}
		
		/**
		 * \brief 销毁
		 */
		virtual void Destroy() {}

		/**
		 * \brief 绘制物体
		 * \param pRenderer renderer
		 */
		virtual void DrawIndexedInstanced(const Renderer* pRenderer) = 0;

	private:
		/**
		 * \brief 单例
		 */
		static CommandManager* m_Instance;
	};
}
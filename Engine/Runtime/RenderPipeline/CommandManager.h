#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Material;
	class Mesh;
	
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
		 * \brief 销毁
		 */
		virtual void Destroy() {}

		/**
		 * \brief 清理缓冲
		 * \param clearColor 是否清理颜色缓冲
		 * \param clearDepth 是否清理深度缓冲
		 * \param color 默认颜色
		 */
		virtual void ClearRenderTargets(bool clearColor, bool clearDepth, const Color& color) = 0;

		/**
		 * \brief 设置根描述符表
		 */
		virtual void SetRootSignature() = 0;

		/**
		 * \brief 绘制物体
		 * \param pMesh Mesh
		 * \param pMaterial Material
		 */
		virtual void DrawIndexedInstanced(const Mesh* pMesh, const Material* pMaterial) = 0;

	private:
		/**
		 * \brief 单例
		 */
		static CommandManager* m_Instance;
	};
}
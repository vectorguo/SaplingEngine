#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Camera;
	class Scene;
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
		 * \brief 获取Object常量缓冲区索引
		 * \return 常量缓冲区索引
		 */
		virtual uint32_t PopObjectCbIndex() = 0;

		/**
		 * \brief 归还常量缓冲区索引
		 * \param index 常量缓冲区索引
		 */
		virtual void PushObjectCbIndex(uint32_t index) = 0;
		
		/**
		 * \brief 更新物体常量缓冲区数据
		 * \param pActiveScene 当前活动场景
		 */
		virtual void UpdateObjectConstantBuffer(Scene* pActiveScene)
		{
			
		}
		
		/**
		 * \brief 更新Pass常量缓冲区数据
		 * \param pCamera 当前相机
		 */
		virtual void UpdatePassConstantBuffer(Camera* pCamera)
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
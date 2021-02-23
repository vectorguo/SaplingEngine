#pragma once

#include "SaplingEnginePch.h"
#include "Camera/CameraManager.h"
#include "Render/RenderPipeline/RenderPipeline.h"

namespace SaplingEngine
{
	class GameSetting final
	{
	public:
		/**
		 * \brief 初始化
		 * \return 是否初始化成功
		 */
		static bool Initialize();

		/**
		 * \brief 获取屏幕宽度
		 * \return 屏幕宽度
		 */
		static uint32_t ScreenWidth()
		{
			return m_ScreenWidth;
		}

		/**
		 * \brief 获取屏幕高度
		 * \return 屏幕高度
		 */
		static uint32_t ScreenHeight()
		{
			return m_ScreenHeight;
		}

		/**
		 * \brief 设置屏幕尺寸
		 * \param width 屏幕宽度
		 * \param height 屏幕高度
		 */
		static void SetScreenSize(uint32_t width, uint32_t height)
		{
			m_ScreenWidth = width;
			m_ScreenHeight = height;

			//通知相机和渲染管线屏幕大小变化
			CameraManager::OnSceneResize(width, height);
			RenderPipeline::Instance()->OnSceneResize(width, height);
		}

		/**
		 * \brief 获取启动场景名称
		 * \return 启动场景名称
		 */
		static const std::string& StartSceneName()
		{
			return m_StartSceneName;
		}
		
		/**
		 * \brief 获取启动场景路径
		 * \return 启动场景路径
		 */
		static const std::string& StartScenePath()
		{
			return m_StartScenePath;
		}

	private:
		/**
		 * \brief 屏幕宽度
		 */
		static uint32_t m_ScreenWidth;
		
		/**
		 * \brief 屏幕高度
		 */
		static uint32_t m_ScreenHeight;

		/**
		 * \brief 起始场景名称
		 */
		static std::string m_StartSceneName;
		
		/**
		 * \brief 起始场景路径
		 */
		static std::string m_StartScenePath;
	};
}

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
			return screenWidth;
		}

		/**
		 * \brief 获取屏幕高度
		 * \return 屏幕高度
		 */
		static uint32_t ScreenHeight()
		{
			return screenHeight;
		}

		/**
		 * \brief 设置屏幕尺寸
		 * \param width 屏幕宽度
		 * \param height 屏幕高度
		 */
		static void SetScreenSize(uint32_t width, uint32_t height)
		{
			screenWidth = width;
			screenHeight = height;

			//通知相机和渲染管线屏幕大小变化
			CameraManager::OnSceneResize(width, height);
			RenderPipeline::OnSceneResize(width, height);
		}

		/**
		 * \brief 获取启动场景名称
		 * \return 启动场景名称
		 */
		static const std::string& StartSceneName()
		{
			return startSceneName;
		}
		
		/**
		 * \brief 获取启动场景路径
		 * \return 启动场景路径
		 */
		static const std::string& StartScenePath()
		{
			return startScenePath;
		}

	private:
		/**
		 * \brief 屏幕宽度
		 */
		static uint32_t screenWidth;
		
		/**
		 * \brief 屏幕高度
		 */
		static uint32_t screenHeight;

		/**
		 * \brief 起始场景名称
		 */
		static std::string startSceneName;
		
		/**
		 * \brief 起始场景路径
		 */
		static std::string startScenePath;
	};
}

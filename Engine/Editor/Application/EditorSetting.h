#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEditor
{
	class EditorSetting
	{
	public:
		/**
		 * \brief	初始化
		 * \param	width		屏幕宽度
		 * \param	height		屏幕高度
		 * \return	是否初始化成功
		 */
		static void Initialize(uint32_t width, uint32_t height);

		/**
		 * \brief	获取编辑器启动场景名称
		 * \return	编辑器启动场景名称
		 */
		inline static const std::string& StartSceneName()
		{
			return startSceneName;
		}

		/**
		 * \brief	获取编辑器启动场景路径
		 * \return	编辑器启动场景路径
		 */
		inline static const std::string& StartScenePath()
		{
			return startScenePath;
		}

	private:
		/**
		 * \brief	编辑器起始场景名称
		 */
		static std::string startSceneName;

		/**
		 * \brief	编辑器起始场景路径
		 */
		static std::string startScenePath;
	};
}
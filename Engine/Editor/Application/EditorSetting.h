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
	};
}
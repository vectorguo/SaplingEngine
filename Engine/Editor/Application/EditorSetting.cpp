#include "Editor/Application/EditorSetting.h"

namespace SaplingEditor
{
	/**
	 * \brief	初始化
	 * \param	width		屏幕宽度
	 * \param	height		屏幕高度
	 * \return	是否初始化成功
	 */
	void EditorSetting::Initialize(uint32_t width, uint32_t height)
	{
		//读取窗口宽度和高度
		screenWidth = width;
		screenHeight = height;
	}
}
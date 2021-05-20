#include "Application/Setting.h"
#include "Editor/Application/EditorSetting.h"

using namespace SaplingEngine;

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
		Setting::Initialize();

		//设置编辑器模式
		Setting::isEditorMode = true;

		//重新设置窗口宽度和高度
		Setting::screenWidth = width;
		Setting::screenHeight = height;
	}
}
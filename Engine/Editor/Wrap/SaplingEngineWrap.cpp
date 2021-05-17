#include "Editor/Wrap/SaplingEngineWrap.h"

using namespace SaplingEngine;

int Test()
{
	return 20;
}

/**
 * \brief	配置信息初始化
 */
SaplingEngineExport bool Setting_Initialize()
{
	return Setting::Initialize();
}

/**
 * \brief	初始化App
 * \param	instanceAddress app句柄
 * \param	hwndAddress		窗口句柄
 * \param	windowWidth		窗口宽度
 * \param	windowHeight	窗口高度
 * \return	是否初始化成功
 */
SaplingEngineExport bool Application_Initialize(int* instanceAddress, int* hwndAddress, int windowWidth, int windowHeight)
{
	return Application::Initialize((HINSTANCE)instanceAddress, (HWND)hwndAddress, windowWidth, windowHeight);
}

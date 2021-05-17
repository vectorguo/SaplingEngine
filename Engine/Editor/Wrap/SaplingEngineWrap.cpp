#include "Editor/Wrap/SaplingEngineWrap.h"

using namespace SaplingEngine;

int Test()
{
	return 20;
}

/**
 * \brief	配置信息初始化
 * \param	screenWidth		窗口宽度
 * \param	screenHeight	窗口高度
 */
SaplingEngineExport bool Setting_Initialize(uint32_t screenWidth, uint32_t screenHeight)
{
	return Setting::Initialize(screenWidth, screenHeight);
}

/**
 * \brief	初始化App
 * \param	instanceAddress app句柄
 * \param	hwndAddress		窗口句柄
 * \return	是否初始化成功
 */
SaplingEngineExport bool Application_Initialize(int* instanceAddress, int* hwndAddress)
{
	return Application::Initialize((HINSTANCE)instanceAddress, (HWND)hwndAddress);
}

/**
 * \brief	Application运行
 */
SaplingEngineExport void Application_Run()
{
	Application::Run();
}

/**
 * \brief	Application销毁
 */
SaplingEngineExport void Application_Destroy()
{
	Application::Destroy();
}

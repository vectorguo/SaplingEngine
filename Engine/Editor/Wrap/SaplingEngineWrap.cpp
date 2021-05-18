#include "Editor/Wrap/SaplingEngineWrap.h"
#include "Editor/Application/EditorApplication.h"
#include "Editor/Application/EditorSetting.h"

using namespace SaplingEditor;
using namespace SaplingEngine;

/**
 * \brief	初始化
 * \param	instanceAddress app句柄
 * \param	hwndAddress		窗口句柄
 * \param	screenWidth		窗口宽度
 * \param	screenHeight	窗口高度
 */
SaplingEngineExport bool Editor_Initialize(int* instanceAddress, int* hwndAddress, uint32_t screenWidth, uint32_t screenHeight)
{
	EditorSetting::Initialize(screenWidth, screenHeight);
	return EditorApplication::Initialize((HINSTANCE)instanceAddress, (HWND)hwndAddress);
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
 * \brief	Application运行一帧
 */
SaplingEngineExport void Application_RunFrame()
{
	Application::RunFrame();
}

/**
 * \brief	Application销毁
 */
SaplingEngineExport void Application_Destroy()
{
	Application::Destroy();
}

/**
 * \brief	Application消息处理
 */
SaplingEngineExport void Application_MessageProcess(int* hWndPtrAddress, int msg, int wParam, int lParam)
{
	Application::MessageProcess((HWND)hWndPtrAddress, msg, wParam, lParam);
}

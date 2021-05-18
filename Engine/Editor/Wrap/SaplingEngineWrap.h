#pragma once

#include "SaplingEngineHeader.h"

#define SaplingEngineExport _declspec(dllexport)

extern "C"
{
	/**
	 * \brief	初始化
	 * \param	instanceAddress app句柄
	 * \param	hwndAddress		窗口句柄
	 * \param	screenWidth		窗口宽度
	 * \param	screenHeight	窗口高度
	 */
	SaplingEngineExport bool Editor_Initialize(int* instanceAddress, int* hwndAddress, uint32_t screenWidth, uint32_t screenHeight);

	/**
	 * \brief	配置信息初始化
	 * \param	screenWidth		窗口宽度
	 * \param	screenHeight	窗口高度
	 */
	SaplingEngineExport bool Setting_Initialize(uint32_t screenWidth, uint32_t screenHeight);

	/**
	 * \brief	初始化App
	 * \param	instanceAddress app句柄
	 * \param	hwndAddress		窗口句柄
	 * \return	是否初始化成功
	 */
	SaplingEngineExport bool Application_Initialize(int* instanceAddress, int* hwndAddress);

	/**
	 * \brief	Application运行
	 */
	SaplingEngineExport void Application_Run();

	/**
	 * \brief	Application运行一帧
	 */
	SaplingEngineExport void Application_RunFrame();

	/**
	 * \brief	Application销毁
	 */
	SaplingEngineExport void Application_Destroy();

	/**
	 * \brief	Application消息处理
	 */
	SaplingEngineExport void Application_MessageProcess(int* hWndPtrAddress, int msg, int wParam, int lParam);
}
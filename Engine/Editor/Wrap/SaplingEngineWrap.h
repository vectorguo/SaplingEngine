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
	 * \brief	Editor运行
	 */
	SaplingEngineExport void Editor_Run();

	/**
	 * \brief	Editor销毁
	 */
	SaplingEngineExport void Editor_Destroy();

	/**
	 * \brief	Editor消息处理
	 */
	SaplingEngineExport void Editor_MessageProcess(int* hWndPtrAddress, int msg, int wParam, int lParam);
}
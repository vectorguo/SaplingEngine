#pragma once

#include "SaplingEngineHeader.h"

#define SaplingEngineExport _declspec(dllexport)

extern "C"
{
	SaplingEngineExport int Test();

	/**
	 * \brief	配置信息初始化
	 */
	SaplingEngineExport bool Setting_Initialize();

	/**
	 * \brief	初始化App
	 * \param	instanceAddress app句柄
	 * \param	hwndAddress		窗口句柄
	 * \param	windowWidth		窗口宽度
	 * \param	windowHeight	窗口高度
	 * \return	是否初始化成功
	 */
	SaplingEngineExport bool Application_Initialize(int* instanceAddress, int* hwndAddress, int windowWidth, int windowHeight);
}
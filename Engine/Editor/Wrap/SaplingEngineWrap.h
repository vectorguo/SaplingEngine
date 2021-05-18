#pragma once

#include "SaplingEngineHeader.h"

#define SaplingEngineExport _declspec(dllexport)

extern "C"
{
	/**
	 * \brief	��ʼ��
	 * \param	instanceAddress app���
	 * \param	hwndAddress		���ھ��
	 * \param	screenWidth		���ڿ��
	 * \param	screenHeight	���ڸ߶�
	 */
	SaplingEngineExport bool Editor_Initialize(int* instanceAddress, int* hwndAddress, uint32_t screenWidth, uint32_t screenHeight);

	/**
	 * \brief	Editor����
	 */
	SaplingEngineExport void Editor_Run();

	/**
	 * \brief	Editor����
	 */
	SaplingEngineExport void Editor_Destroy();

	/**
	 * \brief	Editor��Ϣ����
	 */
	SaplingEngineExport void Editor_MessageProcess(int* hWndPtrAddress, int msg, int wParam, int lParam);
}
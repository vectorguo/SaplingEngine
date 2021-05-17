#pragma once

#include "SaplingEngineHeader.h"

#define SaplingEngineExport _declspec(dllexport)

extern "C"
{
	SaplingEngineExport int Test();

	/**
	 * \brief	������Ϣ��ʼ��
	 * \param	screenWidth		���ڿ��
	 * \param	screenHeight	���ڸ߶�
	 */
	SaplingEngineExport bool Setting_Initialize(uint32_t screenWidth, uint32_t screenHeight);

	/**
	 * \brief	��ʼ��App
	 * \param	instanceAddress app���
	 * \param	hwndAddress		���ھ��
	 * \return	�Ƿ��ʼ���ɹ�
	 */
	SaplingEngineExport bool Application_Initialize(int* instanceAddress, int* hwndAddress);

	/**
	 * \brief	Application����
	 */
	SaplingEngineExport void Application_Run();

	/**
	 * \brief	Application����
	 */
	SaplingEngineExport void Application_Destroy();
}
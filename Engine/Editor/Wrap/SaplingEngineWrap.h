#pragma once

#include "SaplingEngineHeader.h"

#define SaplingEngineExport _declspec(dllexport)

extern "C"
{
	SaplingEngineExport int Test();

	/**
	 * \brief	������Ϣ��ʼ��
	 */
	SaplingEngineExport bool Setting_Initialize();

	/**
	 * \brief	��ʼ��App
	 * \param	instanceAddress app���
	 * \param	hwndAddress		���ھ��
	 * \param	windowWidth		���ڿ��
	 * \param	windowHeight	���ڸ߶�
	 * \return	�Ƿ��ʼ���ɹ�
	 */
	SaplingEngineExport bool Application_Initialize(int* instanceAddress, int* hwndAddress, int windowWidth, int windowHeight);
}
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
	 * \brief	Application����һ֡
	 */
	SaplingEngineExport void Application_RunFrame();

	/**
	 * \brief	Application����
	 */
	SaplingEngineExport void Application_Destroy();

	/**
	 * \brief	Application��Ϣ����
	 */
	SaplingEngineExport void Application_MessageProcess(int* hWndPtrAddress, int msg, int wParam, int lParam);
}
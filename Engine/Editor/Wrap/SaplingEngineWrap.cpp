#include "Editor/Wrap/SaplingEngineWrap.h"

using namespace SaplingEngine;

int Test()
{
	return 20;
}

/**
 * \brief	������Ϣ��ʼ��
 */
SaplingEngineExport bool Setting_Initialize()
{
	return Setting::Initialize();
}

/**
 * \brief	��ʼ��App
 * \param	instanceAddress app���
 * \param	hwndAddress		���ھ��
 * \param	windowWidth		���ڿ��
 * \param	windowHeight	���ڸ߶�
 * \return	�Ƿ��ʼ���ɹ�
 */
SaplingEngineExport bool Application_Initialize(int* instanceAddress, int* hwndAddress, int windowWidth, int windowHeight)
{
	return Application::Initialize((HINSTANCE)instanceAddress, (HWND)hwndAddress, windowWidth, windowHeight);
}

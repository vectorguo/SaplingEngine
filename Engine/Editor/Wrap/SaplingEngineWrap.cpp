#include "Editor/Wrap/SaplingEngineWrap.h"

using namespace SaplingEngine;

int Test()
{
	return 20;
}

/**
 * \brief	������Ϣ��ʼ��
 * \param	screenWidth		���ڿ��
 * \param	screenHeight	���ڸ߶�
 */
SaplingEngineExport bool Setting_Initialize(uint32_t screenWidth, uint32_t screenHeight)
{
	return Setting::Initialize(screenWidth, screenHeight);
}

/**
 * \brief	��ʼ��App
 * \param	instanceAddress app���
 * \param	hwndAddress		���ھ��
 * \return	�Ƿ��ʼ���ɹ�
 */
SaplingEngineExport bool Application_Initialize(int* instanceAddress, int* hwndAddress)
{
	return Application::Initialize((HINSTANCE)instanceAddress, (HWND)hwndAddress);
}

/**
 * \brief	Application����
 */
SaplingEngineExport void Application_Run()
{
	Application::Run();
}

/**
 * \brief	Application����
 */
SaplingEngineExport void Application_Destroy()
{
	Application::Destroy();
}

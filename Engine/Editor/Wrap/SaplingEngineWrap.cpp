#include "Editor/Wrap/SaplingEngineWrap.h"
#include "Editor/Application/EditorApplication.h"
#include "Editor/Application/EditorSetting.h"

using namespace SaplingEditor;
using namespace SaplingEngine;

/**
 * \brief	��ʼ��
 * \param	instanceAddress app���
 * \param	hwndAddress		���ھ��
 * \param	screenWidth		���ڿ��
 * \param	screenHeight	���ڸ߶�
 */
SaplingEngineExport bool Editor_Initialize(int* instanceAddress, int* hwndAddress, uint32_t screenWidth, uint32_t screenHeight)
{
	EditorSetting::Initialize(screenWidth, screenHeight);
	return EditorApplication::Initialize((HINSTANCE)instanceAddress, (HWND)hwndAddress);
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
 * \brief	Application����һ֡
 */
SaplingEngineExport void Application_RunFrame()
{
	Application::RunFrame();
}

/**
 * \brief	Application����
 */
SaplingEngineExport void Application_Destroy()
{
	Application::Destroy();
}

/**
 * \brief	Application��Ϣ����
 */
SaplingEngineExport void Application_MessageProcess(int* hWndPtrAddress, int msg, int wParam, int lParam)
{
	Application::MessageProcess((HWND)hWndPtrAddress, msg, wParam, lParam);
}

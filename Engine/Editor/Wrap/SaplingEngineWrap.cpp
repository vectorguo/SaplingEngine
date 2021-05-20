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
 * \brief	Application����
 */
SaplingEngineExport void Editor_Run()
{
	EditorApplication::Run();
}

/**
 * \brief	Application����
 */
SaplingEngineExport void Editor_Destroy()
{
	EditorApplication::Destroy();
}

/**
 * \brief	Application��Ϣ����
 */
SaplingEngineExport void Editor_MessageProcess(int* hWndPtrAddress, int msg, int wParam, int lParam)
{
	EditorApplication::MessageProcess((HWND)hWndPtrAddress, msg, wParam, lParam);
}

SaplingEngineExport void Scene_OpenScene(const char* sceneName, const char* scenePath)
{
	SceneManager::LoadScene(sceneName, scenePath);
}

SaplingEngineExport void Scene_CloseScene()
{
}

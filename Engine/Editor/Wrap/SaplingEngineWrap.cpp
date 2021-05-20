#include "Editor/Wrap/SaplingEngineWrap.h"

#include "Editor/Application/EditorApplication.h"
#include "Editor/Application/EditorSetting.h"
#include "Editor/Camera/EditorCamera.h"

using namespace SaplingEditor;

/**
 * \brief	初始化
 * \param	instanceAddress app句柄
 * \param	hwndAddress		窗口句柄
 * \param	screenWidth		窗口宽度
 * \param	screenHeight	窗口高度
 */
SaplingEngineExport bool Editor_Initialize(int* instanceAddress, int* hwndAddress, uint32_t screenWidth, uint32_t screenHeight)
{
	EditorSetting::Initialize(screenWidth, screenHeight);
	return EditorApplication::Initialize((HINSTANCE)instanceAddress, (HWND)hwndAddress);
}

/**
 * \brief	Application运行
 */
SaplingEngineExport void Editor_Run()
{
	EditorApplication::Run();
}

/**
 * \brief	Application销毁
 */
SaplingEngineExport void Editor_Destroy()
{
	EditorApplication::Destroy();
}

/**
 * \brief	Application消息处理
 */
SaplingEngineExport void Editor_MessageProcess(int* hWndPtrAddress, int msg, int wParam, int lParam)
{
	EditorApplication::MessageProcess((HWND)hWndPtrAddress, msg, wParam, lParam);
}

SaplingEngineExport void Scene_OpenScene(const char* sceneName, const char* scenePath, Vector3 followOffset, Vector3 targetPosition, Vector3 targetEulerAngles)
{
	SceneManager::LoadScene(sceneName, scenePath);

	//将所有相机设置成不可用状态
	for (auto * pCamera : CameraManager::GetCameras())
	{
		pCamera->SetEnabled(false);
	}
	
	//创建所有Editor对象的根节点
	CreateGameObject(EditorRootObjectName);

	//创建编辑器相机
	EditorCamera::CreateEditorCamera(followOffset, targetPosition, targetEulerAngles);
}

SaplingEngineExport void Scene_CloseScene()
{
}

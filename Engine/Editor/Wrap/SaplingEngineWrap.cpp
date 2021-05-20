#include "Editor/Wrap/SaplingEngineWrap.h"

#include "Editor/Application/EditorApplication.h"
#include "Editor/Application/EditorSetting.h"
#include "Editor/Camera/EditorCamera.h"

using namespace SaplingEditor;

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

SaplingEngineExport void Scene_OpenScene(const char* sceneName, const char* scenePath, Vector3 followOffset, Vector3 targetPosition, Vector3 targetEulerAngles)
{
	SceneManager::LoadScene(sceneName, scenePath);

	//������������óɲ�����״̬
	for (auto * pCamera : CameraManager::GetCameras())
	{
		pCamera->SetEnabled(false);
	}
	
	//��������Editor����ĸ��ڵ�
	CreateGameObject(EditorRootObjectName);

	//�����༭�����
	EditorCamera::CreateEditorCamera(followOffset, targetPosition, targetEulerAngles);
}

SaplingEngineExport void Scene_CloseScene()
{
}

#pragma once

#include "Editor/Utility/EditorUtility.h"

#define SaplingEngineExport _declspec(dllexport)

extern "C"
{
	/**
	 * \brief	初始化
	 * \param	instanceAddress app句柄
	 * \param	hwndAddress		窗口句柄
	 * \param	screenWidth		窗口宽度
	 * \param	screenHeight	窗口高度
	 */
	SaplingEngineExport bool Editor_Initialize(int* instanceAddress, int* hwndAddress, uint32_t screenWidth, uint32_t screenHeight);

	/**
	 * \brief	Editor运行
	 */
	SaplingEngineExport void Editor_Run();

	/**
	 * \brief	Editor销毁
	 */
	SaplingEngineExport void Editor_Destroy();

	/**
	 * \brief	Editor消息处理
	 */
	SaplingEngineExport void Editor_MessageProcess(int* hWndPtrAddress, int msg, int wParam, int lParam);

	/**
	 * \brief	打开场景
	 */
	SaplingEngineExport void Scene_OpenScene(const char* sceneName, const char* scenePath, Vector3 cameraPosition, Vector3 cameraEulerAngles);

	/**
	 * \brief	关闭当前场景
	 */
	SaplingEngineExport void Scene_CloseScene();
}
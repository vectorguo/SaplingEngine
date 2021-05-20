#include "Editor/Camera/EditorCamera.h"

namespace SaplingEditor
{
	CameraSptr						EditorCamera::editorCamera;
	CinemachineVirtualCameraSptr	EditorCamera::editorVirtualCamera;
	GameObjectSptr					EditorCamera::editorCamraObject;
	GameObjectSptr					EditorCamera::editorVirtualCameraObject;
	GameObjectSptr					EditorCamera::editorCamraTargetObject;

	/**
	 * \brief	创建编辑器相机
	 */
	void EditorCamera::CreateEditorCamera(const Vector3& followOffset, const Vector3& targetPosition, const Vector3& targetEulerAngles)
	{
		auto editorRootObject = FindGameObject(EditorRootObjectName);
		auto editorRootTransform = editorRootObject->GetTransform();

		//创建编辑器相机
		editorCamraObject = CreateGameObject("EditorCamera");
		editorCamraObject->GetTransform()->SetParent(editorRootTransform);
		editorCamraObject->AddComponent<CinemachineBrain>();
		editorCamera = editorCamraObject->AddComponent<Camera>();

		//创建虚拟相机目标
		editorCamraTargetObject = CreateGameObject("EditorVirtualCameraTarget");
		editorCamraTargetObject->GetTransform()->SetParent(editorRootTransform);
		editorCamraTargetObject->GetTransform()->SetPosition(targetPosition);
		editorCamraTargetObject->GetTransform()->SetEulerAngles(targetEulerAngles);

		//创建虚拟相机
		editorVirtualCameraObject = CreateGameObject("EditorVirtualCamera");
		editorVirtualCameraObject->GetTransform()->SetParent(editorRootTransform);
		editorVirtualCamera = editorVirtualCameraObject->AddComponent<CinemachineVirtualCamera>();
		editorVirtualCamera->SetFollowTarget(editorCamraTargetObject->GetTransformSptr());
		editorVirtualCamera->SetLookAtTarget(editorCamraTargetObject->GetTransformSptr());
		editorVirtualCamera->SetFollowOffset(followOffset);
	}
}
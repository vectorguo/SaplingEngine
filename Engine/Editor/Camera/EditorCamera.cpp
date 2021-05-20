#include "Editor/Camera/EditorCamera.h"

namespace SaplingEditor
{
	CameraSptr						EditorCamera::editorCamera;
	CinemachineVirtualCameraSptr	EditorCamera::editorVirtualCamera;
	GameObjectSptr					EditorCamera::editorCamraObject;
	GameObjectSptr					EditorCamera::editorVirtualCameraObject;
	GameObjectSptr					EditorCamera::editorCamraTargetObject;

	/**
	 * \brief	�����༭�����
	 */
	void EditorCamera::CreateEditorCamera(const Vector3& followOffset, const Vector3& targetPosition, const Vector3& targetEulerAngles)
	{
		auto editorRootObject = FindGameObject(EditorRootObjectName);
		auto editorRootTransform = editorRootObject->GetTransform();

		//�����༭�����
		editorCamraObject = CreateGameObject("EditorCamera");
		editorCamraObject->GetTransform()->SetParent(editorRootTransform);
		editorCamraObject->AddComponent<CinemachineBrain>();
		editorCamera = editorCamraObject->AddComponent<Camera>();

		//�����������Ŀ��
		editorCamraTargetObject = CreateGameObject("EditorVirtualCameraTarget");
		editorCamraTargetObject->GetTransform()->SetParent(editorRootTransform);
		editorCamraTargetObject->GetTransform()->SetPosition(targetPosition);
		editorCamraTargetObject->GetTransform()->SetEulerAngles(targetEulerAngles);

		//�����������
		editorVirtualCameraObject = CreateGameObject("EditorVirtualCamera");
		editorVirtualCameraObject->GetTransform()->SetParent(editorRootTransform);
		editorVirtualCamera = editorVirtualCameraObject->AddComponent<CinemachineVirtualCamera>();
		editorVirtualCamera->SetFollowTarget(editorCamraTargetObject->GetTransformSptr());
		editorVirtualCamera->SetLookAtTarget(editorCamraTargetObject->GetTransformSptr());
		editorVirtualCamera->SetFollowOffset(followOffset);
	}
}
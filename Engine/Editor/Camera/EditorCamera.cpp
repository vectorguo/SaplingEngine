#include "Editor/Camera/EditorCamera.h"

namespace SaplingEditor
{
	CameraSptr						EditorCamera::editorCamera;
	GameObjectSptr					EditorCamera::editorCamraObject;

	/**
	 * \brief	�����༭�����
	 */
	void EditorCamera::CreateEditorCamera(const Vector3& cameraPosition, const Vector3& cameraEulerAngles)
	{
		auto editorRootObject = FindGameObject(EditorRootObjectName);
		auto editorRootTransform = editorRootObject->GetTransform();

		//�����༭�����
		editorCamraObject = CreateGameObject("EditorCamera");
		editorCamraObject->GetTransform()->SetParent(editorRootTransform);
		editorCamraObject->AddComponent<CinemachineBrain>();
		editorCamera = editorCamraObject->AddComponent<Camera>();
		editorCamera->SetFov(60.0f);
		editorCamera->SetNearClippingPlanes(0.3f);
		editorCamera->SetFarClippingPlanes(1000.0f);

		//��ʼ�����λ�úͷ���
		auto pCameraTransform = editorCamraObject->GetTransform();
		pCameraTransform->SetPosition(cameraPosition);
		pCameraTransform->SetEulerAngles(cameraEulerAngles);

		//�������λ��Ϊ��������
		auto* pActiveScene = SceneManager::GetActiveScene();
		pActiveScene->SetCenterOfSceneBounds(pCameraTransform->GetPosition());
	}

	void EditorCamera::MoveEditorCamera(const Vector3& delta)
	{
		auto* pCameraTransform = editorCamraObject->GetTransform();
		auto forward = pCameraTransform->GetForward();
		forward.y = 0;
		forward.Normalize();

		auto right = pCameraTransform->GetRight();
		
		auto position = pCameraTransform->GetPosition();
		position += forward * delta.y * 0.05f - right * delta.x * 0.05f;
		pCameraTransform->SetPosition(position);

		//�������λ��Ϊ��������
		auto* pActiveScene = SceneManager::GetActiveScene();
		pActiveScene->SetCenterOfSceneBounds(position);
	}
	
	/**
	 * \brief	��ת�༭�����
	 * \param	eularAngles		��ת��ŷ���Ƕ�
	 */
	void EditorCamera::RotateEditorCamera(const Vector3& eularAngles)
	{
		auto* pCameraTransform = editorCamraObject->GetTransform();
		auto cameraRotation = pCameraTransform->GetLocalRotation();
		cameraRotation *= Quaternion::RotateAxis(pCameraTransform->GetRight(), -eularAngles.x * 0.2f);
		cameraRotation *= Quaternion::RotateAxis(Vector3::Up, -eularAngles.y * 0.2f);
		pCameraTransform->SetLocalRotation(cameraRotation);
	}
	
	void EditorCamera::ZoomEditorCamera(float value)
	{
		auto* pCameraTransform = editorCamraObject->GetTransform();
		auto position = pCameraTransform->GetPosition();
		position += pCameraTransform->GetForward() * value * 0.1f;
		pCameraTransform->SetPosition(position);

		//�������λ��Ϊ��������
		auto* pActiveScene = SceneManager::GetActiveScene();
		pActiveScene->SetCenterOfSceneBounds(position);
	}
}
#include "Character/Character.h"
#include "InputSystem.h"
#include "WarcraftGameMain.h"

namespace Warcraft
{
	CinemachineVirtualCameraSptr	InputSystem::characterVirtualCameraSptr;
	CinemachineVirtualCameraSptr	InputSystem::editorVirtualCameraSptr;
	GameObjectSptr					InputSystem::editorTarget;
	bool							InputSystem::isEditorMode = false;

	/**
	 * \brief	��ʼ��
	 * \param	pPlayer		��ҽ�ɫ��ָ��
	 */
	void InputSystem::Initialize(Character* pPlayer)
	{
		//������ɫ�������
		auto pVirtualCameraObj = CreateGameObject("CharacterVirtualCamera");
		characterVirtualCameraSptr = pVirtualCameraObj->AddComponent<CinemachineVirtualCamera>();
		characterVirtualCameraSptr->SetFollowTarget(pPlayer->GetTransformSptr());
		characterVirtualCameraSptr->SetLookAtTarget(pPlayer->GetTransformSptr());
		characterVirtualCameraSptr->SetFollowOffset(Vector3(0, 15, -15));
		characterVirtualCameraSptr->SetPriority(11);

		//�����༭���������Ŀ��
		editorTarget = CreateGameObject("EditorVirtualCameraTarget");

		//�����༭���������
		pVirtualCameraObj = CreateGameObject("EditorVirtualCamera");
		editorVirtualCameraSptr = pVirtualCameraObj->AddComponent<CinemachineVirtualCamera>();
		editorVirtualCameraSptr->SetFollowTarget(editorTarget->GetTransformSptr());
		editorVirtualCameraSptr->SetLookAtTarget(editorTarget->GetTransformSptr());
		editorVirtualCameraSptr->SetFollowOffset(Vector3(0, 150, -150));
		editorVirtualCameraSptr->SetPriority(10);

		isEditorMode = false;
	}

	/**
	 * \brief ����
	 */
	void InputSystem::Update()
	{
		if (isEditorMode)
		{
			UpdateEditorMode();

			if (Input::IsKeyDown(EKeyCode::P))
			{
				isEditorMode = false;
				characterVirtualCameraSptr->SetPriority(11);
				editorVirtualCameraSptr->SetPriority(10);
			}
		}
		else
		{
			UpdateCharacterMode();

			if (Input::IsKeyDown(EKeyCode::P))
			{
				isEditorMode = true;
				characterVirtualCameraSptr->SetPriority(10);
				editorVirtualCameraSptr->SetPriority(11);
			}
		}
	}
	
	/**
	 * \brief	���½�ɫģʽ
	 */
	void InputSystem::UpdateCharacterMode()
	{
		Vector3 offset;
		if (Input::IsKeyPress(EKeyCode::W))
		{
			offset += Vector3::Forward * Time::DeltaTime();
		}
		if (Input::IsKeyPress(EKeyCode::S))
		{
			offset -= Vector3::Forward * Time::DeltaTime();
		}
		if (Input::IsKeyPress(EKeyCode::A))
		{
			offset -= Vector3::Right * Time::DeltaTime();
		}
		if (Input::IsKeyPress(EKeyCode::D))
		{
			offset += Vector3::Right * Time::DeltaTime();
		}
		
		auto* pPlayer = WarcraftGameMain::Instance()->GetPlayer();
		auto* pPlayerTransform = pPlayer->GetTransform();
		pPlayerTransform->SetPosition(pPlayerTransform->GetPosition() + offset);
	}
	
	/**
	 * \brief	���±༭��ģʽ
	 */
	void InputSystem::UpdateEditorMode()
	{
	}
}

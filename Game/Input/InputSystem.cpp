#include "Game/Character/Character.h"
#include "Game/Input/InputSystem.h"
#include "Game/WarcraftGameMain.h"

namespace Warcraft
{
	CinemachineVirtualCameraSptr	InputSystem::characterVirtualCameraSptr;
	CinemachineVirtualCameraSptr	InputSystem::editorVirtualCameraSptr;
	GameObjectSptr					InputSystem::editorTarget;
	bool							InputSystem::isEditorMode = false;

	/**
	 * \brief	初始化
	 * \param	pPlayer		玩家角色的指针
	 */
	void InputSystem::Initialize(Character* pPlayer)
	{
		//创建角色虚拟相机
		auto pVirtualCameraObj = CreateGameObject("CharacterVirtualCamera");
		characterVirtualCameraSptr = pVirtualCameraObj->AddComponent<CinemachineVirtualCamera>();
		characterVirtualCameraSptr->SetFollowTarget(pPlayer->GetTransformSptr());
		characterVirtualCameraSptr->SetLookAtTarget(pPlayer->GetTransformSptr());
		characterVirtualCameraSptr->SetFollowOffset(Vector3(0, 10, -10));
		characterVirtualCameraSptr->SetPriority(11);

		//创建编辑器虚拟相机目标
		editorTarget = CreateGameObject("EditorVirtualCameraTarget");
		editorTarget->GetTransform()->SetPosition(0, 0, -120);

		//创建编辑器虚拟相机
		pVirtualCameraObj = CreateGameObject("EditorVirtualCamera");
		editorVirtualCameraSptr = pVirtualCameraObj->AddComponent<CinemachineVirtualCamera>();
		editorVirtualCameraSptr->SetFollowTarget(editorTarget->GetTransformSptr());
		editorVirtualCameraSptr->SetLookAtTarget(editorTarget->GetTransformSptr());
		editorVirtualCameraSptr->SetFollowOffset(Vector3(0, 50, -50));
		editorVirtualCameraSptr->SetPriority(10);

		isEditorMode = false;
	}

	/**
	 * \brief 更新
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
	 * \brief	更新角色模式
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
		pPlayerTransform->SetPosition(pPlayerTransform->GetPosition() + offset * 5);

		//设置场景包围盒中心点
		auto* pActiveScene = SceneManager::GetActiveScene();
		pActiveScene->SetCenterOfSceneBounds(pPlayerTransform->GetPosition());
	}
	
	/**
	 * \brief	更新编辑器模式
	 */
	void InputSystem::UpdateEditorMode()
	{
	}
}

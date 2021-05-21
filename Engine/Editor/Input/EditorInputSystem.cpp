#include "Editor/Camera/EditorCamera.h"
#include "Editor/Input/EditorInputSystem.h"

namespace SaplingEditor
{
	Vector2 EditorInputSystem::mousePosition;
	EditorInputSystem::EOperationType EditorInputSystem::operationType = EditorInputSystem::EOperationType::ENone;

	/**
	 * \brief ¸üÐÂ
	 */
	void EditorInputSystem::Update()
	{
		auto mouseState = Input::GetMouseButtonState();
		if (operationType == EOperationType::ENone)
		{
			if (mouseState == EMouseButtonState::LeftMouseButtonDown)
			{
				mousePosition = Input::GetMousePosition();
				if (Input::IsKeyPress(EKeyCode::Control) && Input::IsKeyPress(EKeyCode::Shift))
				{
					operationType = EOperationType::EZoom;
				}
				else if (Input::IsKeyPress(EKeyCode::Control))
				{
					operationType = EOperationType::ERotate;
				}
				else
				{
					operationType = EOperationType::ETranslate;
				}
			}
		}
		else
		{
			if (mouseState == EMouseButtonState::MouseButtonMove)
			{
				auto& currentMousePosition = Input::GetMousePosition();
				auto delta = currentMousePosition - mousePosition;
				if (operationType == EOperationType::ETranslate)
				{
					EditorCamera::MoveEditorCamera(Vector3(delta.x, delta.y, 0));
				}
				else if (operationType == EOperationType::ERotate)
				{
					EditorCamera::RotateEditorCamera(Vector3(delta.y, delta.x, 0));
				}
				else
				{
					EditorCamera::ZoomEditorCamera(delta.x);
				}
				mousePosition = currentMousePosition;
			}
			else if (mouseState == EMouseButtonState::LeftMouseButtonUp)
			{
				mousePosition = Vector2::Zero;
				operationType = EOperationType::ENone;
			}
		}
	}
}
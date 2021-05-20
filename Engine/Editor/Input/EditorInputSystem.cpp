#include "Editor/Camera/EditorCamera.h"
#include "Editor/Input/EditorInputSystem.h"

namespace SaplingEditor
{
	Vector2 EditorInputSystem::mousePosition;;

	/**
	 * \brief ¸üÐÂ
	 */
	void EditorInputSystem::Update()
	{
		auto mouseState = Input::GetMouseButtonState();
		if (mouseState == EMouseButtonState::MouseWheel)
		{
			EditorCamera::ZoomEditorCamera(Input::GetMouseWheelValue());
		}
		else
		{
			if (mouseState == EMouseButtonState::LeftMouseButtonDown)
			{
				mousePosition = Input::GetMousePosition();
			}
			else if (mouseState == EMouseButtonState::MouseButtonMove)
			{
				auto& currentMousePosition = Input::GetMousePosition();
				auto delta = currentMousePosition - mousePosition;
				//EditorCamera::RotateEditorCamera(Vector3(delta.y, delta.x, 0));
				EditorCamera::MoveEditorCamera(Vector3(delta.x, delta.y, 0));
				mousePosition = currentMousePosition;
			}
			else if (mouseState == EMouseButtonState::LeftMouseButtonUp)
			{
				mousePosition = Vector2::Zero;
			}
		}
	}
}
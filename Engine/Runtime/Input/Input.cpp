#include "Input.h"

namespace SaplingEngine
{
	//��̬��Ա��ʼ��
	EKeyState			Input::keyState = EKeyState::None;
	EKeyCode			Input::keyCode = EKeyCode::None;

	EMouseButtonState	Input::mouseButtonState = EMouseButtonState::None;
	Vector2				Input::mousePosition(0, 0);
	float				Input::mouseWheelValue = 0;
	
	/**
	 * \brief ����
	 */
	void Input::Destroy()
	{
		
	}

	/**
	* \brief ����
	*/
	void Input::Reset()
	{
		if (keyState == EKeyState::KeyDown)
		{
			keyState = EKeyState::KeyPress;
		}
		if (keyState == EKeyState::KeyUp)
		{
			keyState = EKeyState::None;
			keyCode = EKeyCode::None;
		}

		if (mouseButtonState == EMouseButtonState::LeftMouseButtonUp ||
			mouseButtonState == EMouseButtonState::RightMouseButtonUp)
		{
			mouseButtonState = EMouseButtonState::None;
		}
		else if (mouseButtonState == EMouseButtonState::MouseWheel)
		{
			mouseButtonState = EMouseButtonState::None;
			mouseWheelValue = 0;
		}
	}

	/**
	* \brief	���ð���״̬�밴����
	* \param	state		����״̬
	* \param	code		������
	*/
	void Input::SetKeyState(EKeyState state, EKeyCode code)
	{
		if (state == EKeyState::KeyUp)
		{
			keyState = state;
			keyCode = code;
		}
		else
		{
			if (keyCode == code)
			{
				keyState = EKeyState::KeyPress;
			}
			else
			{
				keyState = EKeyState::KeyDown;
				keyCode = code;
			}
		}
	}

	/**
	 * \brief �������״̬��λ��
	 * \param buttonState ��갴ť״̬
	 * \param x ���X��λ��
	 * \param y ���Y��λ��
	 */
	void Input::SetMouseButton(EMouseButtonState buttonState, int32_t x, int32_t y)
	{
		if (buttonState == EMouseButtonState::MouseButtonMove && (mouseButtonState != EMouseButtonState::LeftMouseButtonDown && mouseButtonState != EMouseButtonState::RightMouseButtonDown && mouseButtonState != EMouseButtonState::MouseButtonMove))
		{
			return;
		}

		if (buttonState == EMouseButtonState::MouseWheel)
		{
			mouseButtonState = buttonState;
			mouseWheelValue = x > 0 ? 1.0f : -1.0f;
		}
		else
		{
			mouseButtonState = buttonState;
			mousePosition.x = static_cast<float>(x);
			mousePosition.y = static_cast<float>(y);
		}
	}
}

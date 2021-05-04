#include "Input.h"

namespace SaplingEngine
{
	//静态成员初始化
	EKeyState			Input::keyState = EKeyState::None;
	EKeyCode			Input::keyCode = EKeyCode::None;

	EMouseButtonState	Input::mouseButtonState = EMouseButtonState::None;
	Vector2				Input::mousePosition(0, 0);
	float				Input::mouseWheelValue = 0;
	
	/**
	 * \brief 销毁
	 */
	void Input::Destroy()
	{
		
	}

	/**
	* \brief 重置
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
	* \brief	设置按键状态与按键码
	* \param	state		按键状态
	* \param	code		按键码
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
	 * \brief 设置鼠标状态与位置
	 * \param buttonState 鼠标按钮状态
	 * \param x 鼠标X轴位置
	 * \param y 鼠标Y轴位置
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

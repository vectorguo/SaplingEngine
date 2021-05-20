#include "Input.h"

namespace SaplingEngine
{
	//静态成员初始化
	std::map<EKeyCode, EKeyState> Input::keyMap;

	EMouseButtonState	Input::tempMouseButtonState = EMouseButtonState::None;
	Vector2				Input::tempMousePosition;
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
	 * \brief 刷新
	 */
	void Input::Refresh()
	{
		if (tempMouseButtonState == EMouseButtonState::None)
		{
			return;
		}

		if (tempMouseButtonState == EMouseButtonState::MouseButtonMove && (
			mouseButtonState != EMouseButtonState::LeftMouseButtonDown &&
			mouseButtonState != EMouseButtonState::RightMouseButtonDown &&
			mouseButtonState != EMouseButtonState::MouseButtonMove))
		{
			return;
		}

		mouseButtonState = tempMouseButtonState;
		if (mouseButtonState != EMouseButtonState::MouseWheel)
		{
			mousePosition = tempMousePosition;
		}
	}

	/**
	* \brief 重置
	*/
	void Input::Reset()
	{
		//更新键盘状态
		for (auto iter = keyMap.begin(); iter != keyMap.end();)
		{
			if (iter->second == EKeyState::KeyUp)
			{
				iter = keyMap.erase(iter);
			}
			else
			{
				if (iter->second == EKeyState::KeyDown)
				{
					iter->second = EKeyState::KeyPress;
				}
				++iter;
			}
		}

		//更新鼠标状态
		tempMouseButtonState = EMouseButtonState::None;
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
			keyMap[code] = state;
		}
		else
		{
			auto iter = keyMap.find(code);
			if (iter == keyMap.end())
			{
				keyMap.emplace(code, state);
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
		if (tempMouseButtonState != EMouseButtonState::None && buttonState == EMouseButtonState::MouseButtonMove)
		{
			return;
		}

		tempMouseButtonState = buttonState;
		if (tempMouseButtonState == EMouseButtonState::MouseWheel)
		{
			mouseWheelValue = x > 0 ? 1.0f : -1.0f;
		}
		else
		{
			tempMousePosition.x = static_cast<float>(x);
			tempMousePosition.y = static_cast<float>(y);
		}
	}
}

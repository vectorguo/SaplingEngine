#include "Input.h"

namespace SaplingEngine
{
	//��̬��Ա��ʼ��
	std::map<EKeyCode, EKeyState> Input::keyMap;

	EMouseButtonState	Input::tempMouseButtonState = EMouseButtonState::None;
	Vector2				Input::tempMousePosition;
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
	 * \brief ˢ��
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
	* \brief ����
	*/
	void Input::Reset()
	{
		//���¼���״̬
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

		//�������״̬
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
	* \brief	���ð���״̬�밴����
	* \param	state		����״̬
	* \param	code		������
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
	 * \brief �������״̬��λ��
	 * \param buttonState ��갴ť״̬
	 * \param x ���X��λ��
	 * \param y ���Y��λ��
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

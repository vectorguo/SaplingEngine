#include "Input.h"

namespace SaplingEngine
{
	/**
	 * \brief 销毁
	 */
	void Input::Destroy()
	{
		
	}

	/**
	 * \brief 设置鼠标状态与位置
	 * \param buttonState 鼠标按钮状态
	 * \param x 鼠标X轴位置
	 * \param y 鼠标Y轴位置
	 */
	void Input::SetMouseButton(EMouseButtonState buttonState, int32_t x, int32_t y)
	{
		if (buttonState == EMouseButtonState::MouseButtonMove && (m_MouseButtonState != EMouseButtonState::LeftMouseButtonDown && m_MouseButtonState != EMouseButtonState::RightMouseButtonDown && m_MouseButtonState != EMouseButtonState::MouseButtonMove))
		{
			return;
		}

		if (buttonState == EMouseButtonState::MouseWheel)
		{
			m_MouseButtonState = buttonState;
			m_MouseWheelValue = x > 0 ? 1.0f : -1.0f;
		}
		else
		{
			m_MouseButtonState = buttonState;
			m_MousePosition.x = static_cast<float>(x);
			m_MousePosition.y = static_cast<float>(y);
		}
	}
}

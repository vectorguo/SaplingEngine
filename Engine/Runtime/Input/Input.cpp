#include "Input.h"

namespace SaplingEngine
{
	/**
	 * \brief ����
	 */
	void Input::Destroy()
	{
		
	}

	/**
	 * \brief �������״̬��λ��
	 * \param buttonState ��갴ť״̬
	 * \param x ���X��λ��
	 * \param y ���Y��λ��
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

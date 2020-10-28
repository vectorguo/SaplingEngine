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
		if (buttonState == EMouseButtonState::MouseButtonMove && m_MouseButtonState == EMouseButtonState::None)
		{
			return;
		}
		m_MouseButtonState = buttonState;
		m_MousePosition.x = static_cast<float>(x);
		m_MousePosition.y = static_cast<float>(y);
	}
}

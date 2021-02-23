#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	enum class EMouseButtonState
	{
		None,
		
		LeftMouseButtonDown,
		LeftMouseButtonUp,

		RightMouseButtonDown,
		RightMouseButtonUp,

		MouseButtonMove,
		MouseWheel,
	};

	enum class EKeyState
	{
		None,

		KeyDown,
		KeyUp,
		KeyPress,
	};

	enum class EKeyCode
	{
		
	};
	
	class Input final
	{
		friend class GameApplication;
		
	public:
		/**
		 * \brief ����
		 */
		static void Destroy();
		
		/**
		 * \brief ��ȡ��갴ť״̬
		 * \return ��갴ť״̬
		 */
		static EMouseButtonState GetMouseButtonState()
		{
			return m_MouseButtonState;
		}
		
		/**
		 * \brief ��ȡ���λ��
		 * \return ���λ��
		 */
		static const Vector2& GetMousePosition()
		{
			return m_MousePosition;
		}

		/**
		 * \brief ��ȡ�����ֵĹ���ֵ
		 * \return ����ֵ
		 */
		static float GetMouseWheelValue()
		{
			return m_MouseWheelValue;
		}

	private:
		/**
		 * \brief ����
		 */
		static void Reset()
		{
			if (m_MouseButtonState == EMouseButtonState::LeftMouseButtonUp ||
				m_MouseButtonState == EMouseButtonState::RightMouseButtonUp)
			{
				m_MouseButtonState = EMouseButtonState::None;
			}
			else if (m_MouseButtonState == EMouseButtonState::MouseWheel)
			{
				m_MouseButtonState = EMouseButtonState::None;
				m_MouseWheelValue = 0;
			}
		}
		
		/**
		 * \brief �������״̬��λ��
		 * \param buttonState ��갴ť״̬
		 * \param x ���X��λ��
		 * \param y ���Y��λ��
		 */
		static void SetMouseButton(EMouseButtonState buttonState, int32_t x, int32_t y);
		
	private:
		static EMouseButtonState m_MouseButtonState;
		static Vector2 m_MousePosition;
		static float m_MouseWheelValue;
	};
}

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

		MouseButtonMove
	};

	enum class EKeyState
	{
		None,

		KeyDown,
		KeyUp,
		KeyPress,
	};
	
	class Input final
	{
		friend class GameApplication;
		
	public:
		Input(): m_MousePosition(0, 0)
		{
			
		}

		~Input() = default;

		SINGLETON(Input)

		/**
		 * \brief ����
		 */
		void Destroy();
		
		/**
		 * \brief ��ȡ��갴ť״̬
		 * \return ��갴ť״̬
		 */
		EMouseButtonState GetMouseButtonState() const
		{
			return m_MouseButtonState;
		}
		
		/**
		 * \brief ��ȡ���λ��
		 * \return ���λ��
		 */
		const Vector2& GetMousePosition() const
		{
			return m_MousePosition;
		}

	private:
		/**
		 * \brief ����
		 */
		void Reset()
		{
			if (m_MouseButtonState == EMouseButtonState::LeftMouseButtonUp ||
				m_MouseButtonState == EMouseButtonState::RightMouseButtonUp)
			{
				m_MouseButtonState = EMouseButtonState::None;
			}
		}
		
		/**
		 * \brief �������״̬��λ��
		 * \param buttonState ��갴ť״̬
		 * \param x ���X��λ��
		 * \param y ���Y��λ��
		 */
		void SetMouseButton(EMouseButtonState buttonState, int32_t x, int32_t y);
		
	private:
		EMouseButtonState m_MouseButtonState = EMouseButtonState::None;
		Vector2 m_MousePosition;
	};
}

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
		Input(): m_MousePosition(0, 0)
		{
			
		}

		~Input() = default;

		SINGLETON(Input)

		/**
		 * \brief 销毁
		 */
		void Destroy();
		
		/**
		 * \brief 获取鼠标按钮状态
		 * \return 鼠标按钮状态
		 */
		EMouseButtonState GetMouseButtonState() const
		{
			return m_MouseButtonState;
		}
		
		/**
		 * \brief 获取鼠标位置
		 * \return 鼠标位置
		 */
		const Vector2& GetMousePosition() const
		{
			return m_MousePosition;
		}

		/**
		 * \brief 获取鼠标滚轮的滚动值
		 * \return 滚动值
		 */
		float GetMouseWheelValue() const
		{
			return m_MouseWheelValue;
		}

	private:
		/**
		 * \brief 重置
		 */
		void Reset()
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
		 * \brief 设置鼠标状态与位置
		 * \param buttonState 鼠标按钮状态
		 * \param x 鼠标X轴位置
		 * \param y 鼠标Y轴位置
		 */
		void SetMouseButton(EMouseButtonState buttonState, int32_t x, int32_t y);
		
	private:
		EMouseButtonState m_MouseButtonState = EMouseButtonState::None;
		Vector2 m_MousePosition;
		float m_MouseWheelValue = 0;
	};
}

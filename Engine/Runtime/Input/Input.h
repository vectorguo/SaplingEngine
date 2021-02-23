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
		 * \brief 销毁
		 */
		static void Destroy();
		
		/**
		 * \brief 获取鼠标按钮状态
		 * \return 鼠标按钮状态
		 */
		static EMouseButtonState GetMouseButtonState()
		{
			return m_MouseButtonState;
		}
		
		/**
		 * \brief 获取鼠标位置
		 * \return 鼠标位置
		 */
		static const Vector2& GetMousePosition()
		{
			return m_MousePosition;
		}

		/**
		 * \brief 获取鼠标滚轮的滚动值
		 * \return 滚动值
		 */
		static float GetMouseWheelValue()
		{
			return m_MouseWheelValue;
		}

	private:
		/**
		 * \brief 重置
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
		 * \brief 设置鼠标状态与位置
		 * \param buttonState 鼠标按钮状态
		 * \param x 鼠标X轴位置
		 * \param y 鼠标Y轴位置
		 */
		static void SetMouseButton(EMouseButtonState buttonState, int32_t x, int32_t y);
		
	private:
		static EMouseButtonState m_MouseButtonState;
		static Vector2 m_MousePosition;
		static float m_MouseWheelValue;
	};
}

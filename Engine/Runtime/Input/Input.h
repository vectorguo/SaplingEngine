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
		 * \brief 销毁
		 */
		void Destroy();
		
		/**
		 * \brief 获取鼠标按钮状态
		 * \return 鼠标按钮状态
		 */
		EMouseButtonState MouseButtonState() const
		{
			return m_MouseButtonState;
		}
		
		/**
		 * \brief 获取鼠标位置
		 * \return 鼠标位置
		 */
		const Vector2& MousePosition() const
		{
			return m_MousePosition;
		}

	private:
		/**
		 * \brief 重置
		 */
		void Reset()
		{
			m_MouseButtonState = EMouseButtonState::None;
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
	};
}

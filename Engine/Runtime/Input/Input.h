#pragma once

#include "Runtime/SaplingEnginePch.h"

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
		KeyDown,
		KeyUp,
		KeyPress,
	};

	enum class EKeyCode : uint32_t
	{
		None	= 0x00,
		Shift	= 0x10,
		Control = 0x11,
		Alt		= 0x12,
		Space	= 0x20,
		Zero	= 0x30,
		One		= 0x31,
		Two		= 0x02,
		Three	= 0x33,
		Four	= 0x34,
		Five	= 0x35,
		Six		= 0x36,
		Seven	= 0x37,
		Eight	= 0x38,
		Nine	= 0x39,
		A		= 0x41,
		B		= 0x42,
		C		= 0x43,
		D		= 0x44,
		E		= 0x45,
		F		= 0x46,
		G		= 0x47,
		H		= 0x48,
		I		= 0x49,
		J		= 0x4A,
		K		= 0x4B,
		L		= 0x4C,
		M		= 0x4D,
		N		= 0x4E,
		O		= 0x4F,
		P		= 0x50,
		Q		= 0x51,
		R		= 0x52,
		S		= 0x53,
		T		= 0x54,
		U		= 0x55,
		V		= 0x56,
		W		= 0x57,
		X		= 0x58,
		Y		= 0x59,
		Z		= 0x5A,
	};

	class Input final
	{
		friend class Application;

#if SAPLING_EDITOR
		friend class SaplingEditor::EditorApplication;
#endif

	public:
		/**
		 * \brief 销毁
		 */
		static void Destroy();
		
		/**
		 * \brief	按键是否按下
		 * \param	code		按键码
		 * \return	是否按下
		 */
		static bool IsKeyDown(EKeyCode code)
		{
			auto iter = keyMap.find(code);
			return iter != keyMap.end() && iter->second == EKeyState::KeyDown;
		}

		/**
		 * \brief	按键是否按住
		 * \param	code		按键码
		 * \return	是否按住
		 */
		static bool IsKeyPress(EKeyCode code)
		{
			auto iter = keyMap.find(code);
			return iter != keyMap.end() && iter->second == EKeyState::KeyPress;
		}

		/**
		 * \brief	按键是否松开
		 * \param	code		按键码
		 * \return	是否松开
		 */
		static bool IsKeyUp(EKeyCode code)
		{
			auto iter = keyMap.find(code);
			return iter != keyMap.end() && iter->second == EKeyState::KeyUp;
		}

		/**
		 * \brief 获取鼠标按钮状态
		 * \return 鼠标按钮状态
		 */
		static EMouseButtonState GetMouseButtonState()
		{
			return mouseButtonState;
		}
		
		/**
		 * \brief 获取鼠标位置
		 * \return 鼠标位置
		 */
		static const Vector2& GetMousePosition()
		{
			return mousePosition;
		}

		/**
		 * \brief 获取鼠标滚轮的滚动值
		 * \return 滚动值
		 */
		static float GetMouseWheelValue()
		{
			return mouseWheelValue;
		}

	private:
		/**
		 * \brief 刷新
		 */
		static void Refresh();

		/**
		 * \brief 重置
		 */
		static void Reset();
		
		/**
		 * \brief	设置按键状态与按键码
		 * \param	state		按键状态
		 * \param	code		按键码
		 */
		static void SetKeyState(EKeyState state, EKeyCode code);

		/**
		 * \brief	设置鼠标状态与位置
		 * \param	buttonState	鼠标按钮状态
		 * \param	x			鼠标X轴位置
		 * \param	y			鼠标Y轴位置
		 */
		static void SetMouseButton(EMouseButtonState buttonState, int32_t x, int32_t y);
		
	private:
		static std::map<EKeyCode, EKeyState> keyMap;

		static EMouseButtonState tempMouseButtonState;
		static Vector2 tempMousePosition;
		static EMouseButtonState mouseButtonState;
		static Vector2 mousePosition;
		static float mouseWheelValue;
	};
}

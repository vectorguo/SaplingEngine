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
		 * \brief ����
		 */
		static void Destroy();
		
		/**
		 * \brief	�����Ƿ���
		 * \param	code		������
		 * \return	�Ƿ���
		 */
		static bool IsKeyDown(EKeyCode code)
		{
			auto iter = keyMap.find(code);
			return iter != keyMap.end() && iter->second == EKeyState::KeyDown;
		}

		/**
		 * \brief	�����Ƿ�ס
		 * \param	code		������
		 * \return	�Ƿ�ס
		 */
		static bool IsKeyPress(EKeyCode code)
		{
			auto iter = keyMap.find(code);
			return iter != keyMap.end() && iter->second == EKeyState::KeyPress;
		}

		/**
		 * \brief	�����Ƿ��ɿ�
		 * \param	code		������
		 * \return	�Ƿ��ɿ�
		 */
		static bool IsKeyUp(EKeyCode code)
		{
			auto iter = keyMap.find(code);
			return iter != keyMap.end() && iter->second == EKeyState::KeyUp;
		}

		/**
		 * \brief ��ȡ��갴ť״̬
		 * \return ��갴ť״̬
		 */
		static EMouseButtonState GetMouseButtonState()
		{
			return mouseButtonState;
		}
		
		/**
		 * \brief ��ȡ���λ��
		 * \return ���λ��
		 */
		static const Vector2& GetMousePosition()
		{
			return mousePosition;
		}

		/**
		 * \brief ��ȡ�����ֵĹ���ֵ
		 * \return ����ֵ
		 */
		static float GetMouseWheelValue()
		{
			return mouseWheelValue;
		}

	private:
		/**
		 * \brief ˢ��
		 */
		static void Refresh();

		/**
		 * \brief ����
		 */
		static void Reset();
		
		/**
		 * \brief	���ð���״̬�밴����
		 * \param	state		����״̬
		 * \param	code		������
		 */
		static void SetKeyState(EKeyState state, EKeyCode code);

		/**
		 * \brief	�������״̬��λ��
		 * \param	buttonState	��갴ť״̬
		 * \param	x			���X��λ��
		 * \param	y			���Y��λ��
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

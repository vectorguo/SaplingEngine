#pragma once

#include "Editor/Utility/EditorUtility.h"

namespace SaplingEditor
{
	class EditorInputSystem
	{
	public:
		/**
		 * \brief	����
		 */
		static void Update();

	private:
		/**
		 * \brief	���λ��
		 */
		static Vector2 mousePosition;
	};
}
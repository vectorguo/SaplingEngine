#pragma once

#include "Editor/Utility/EditorUtility.h"

namespace SaplingEditor
{
	class EditorInputSystem
	{
	public:
		/**
		 * \brief	更新
		 */
		static void Update();

	private:
		/**
		 * \brief	鼠标位置
		 */
		static Vector2 mousePosition;
	};
}
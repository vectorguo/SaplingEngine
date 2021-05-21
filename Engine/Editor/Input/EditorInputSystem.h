#pragma once

#include "Editor/Utility/EditorUtility.h"

namespace SaplingEditor
{
	class EditorInputSystem
	{
		enum class EOperationType : int8_t
		{
			ENone = 0,
			ETranslate = 1,
			ERotate = 2,
			EZoom = 3,
		};

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

		/**
		 * \brief	操作类型(0:空 1:平移 2:旋转)
		 */
		static EOperationType operationType;
	};
}
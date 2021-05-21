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
		 * \brief	����
		 */
		static void Update();

	private:
		/**
		 * \brief	���λ��
		 */
		static Vector2 mousePosition;

		/**
		 * \brief	��������(0:�� 1:ƽ�� 2:��ת)
		 */
		static EOperationType operationType;
	};
}
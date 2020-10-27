#pragma once

#include "ChessWarEnum.h"
#include "SaplingEngineHeader.h"
#include "Input/ChessWarInput.h"

namespace ChessWar
{
	/**
	 * \brief ����������ʹ������
	 * \param componentType �������
	 * \param componentSubType ���������
	 * \return ���ָ��
	 */
	inline SaplingEngine::Component* CreateComponent(uint32_t componentType, int32_t componentSubType = 0)
	{
		switch (componentType)
		{
		case ComponentType_ChessWarInput:
			return new ChessWarInput();
		default:
			return nullptr;
		}
	}
}

#pragma once

#include "Character/Character.h"
#include "Input/InputSystem.h"
#include "SaplingEngineHeader.h"
#include "WarcraftEnum.h"
#include "WarcraftGameMain.h"

namespace Warcraft
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
			case ComponentType_WarcraftGameMain:
				return new WarcraftGameMain();
			case ComponentType_Character:
				return new Character();
			default:
				return nullptr;
		}
	}
}

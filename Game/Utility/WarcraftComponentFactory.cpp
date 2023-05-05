#pragma once

#include "Runtime/Component/ComponentFactory.h"

#include "Game/Character/Character.h"
#include "Game/WarcraftEnum.h"
#include "Game/WarcraftGameMain.h"

using namespace Warcraft;

namespace SaplingEngine
{
	/**
	 * \brief ����������ʹ����û��Զ������
	 * \param componentType �������
	 * \param componentSubType ���������
	 * \return ���ָ��
	 */
	ComponentSptr ComponentFactory::CreateCustomComponent(uint32_t componentType, int32_t componentSubType)
	{
		switch (componentType)
		{
			case ComponentType_WarcraftGameMain:
				return MakeShared<WarcraftGameMain>();
			case ComponentType_Character:
				return MakeShared<Character>();
			default:
				return nullptr;
		}
	}
}

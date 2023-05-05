#pragma once

#include "Runtime/Component/ComponentFactory.h"

#include "Game/Character/Character.h"
#include "Game/WarcraftEnum.h"
#include "Game/WarcraftGameMain.h"

using namespace Warcraft;

namespace SaplingEngine
{
	/**
	 * \brief 根据组件类型创建用户自定义组件
	 * \param componentType 组件类型
	 * \param componentSubType 组件子类型
	 * \return 组件指针
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

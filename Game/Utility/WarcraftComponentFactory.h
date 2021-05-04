#pragma once

#include "Character/Character.h"
#include "Input/InputSystem.h"
#include "SaplingEngineHeader.h"
#include "WarcraftEnum.h"
#include "WarcraftGameMain.h"

namespace Warcraft
{
	/**
	 * \brief 根据组件类型创建组件
	 * \param componentType 组件类型
	 * \param componentSubType 组件子类型
	 * \return 组件指针
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

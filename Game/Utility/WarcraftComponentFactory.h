#pragma once

#include "Input/WarcraftInput.h"
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
			case ComponentType_WarcraftInput:
				return new WarcraftInput();
			default:
				return nullptr;
		}
	}
}

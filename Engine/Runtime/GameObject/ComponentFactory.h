#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class ComponentFactory final
	{
		friend class GameObject;
		
	private:
		/**
		 * \brief 根据组件类型创建组件
		 * \param componentType 组件类型
		 * \param componentSubType 组件子类型
		 * \return 组件指针
		 */
		static ComponentSptr CreateComponent(uint32_t componentType, int32_t componentSubType = 0);

		/**
		 * \brief 根据组件类型创建用户自定义组件
		 * \param componentType 组件类型
		 * \param componentSubType 组件子类型
		 * \return 组件指针
		 */
		static ComponentSptr CreateCustomComponent(uint32_t componentType, int32_t componentSubType = 0);
	};
}
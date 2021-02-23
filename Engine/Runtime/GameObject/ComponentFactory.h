#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Component;
	
	class ComponentFactory final
	{
		friend class GameObject;
	
	public:
		/**
		 * \brief 设置组件构建器
		 * \param creator 组件构建器
		 */
		static void SetComponentCreator(const std::function<Component* (uint32_t, int32_t)>& creator)
		{
			m_CustomCreator = creator;
		}
		
	private:
		/**
		 * \brief 根据组件类型创建组件
		 * \param componentType 组件类型
		 * \param componentSubType 组件子类型
		 * \return 组件指针
		 */
		static Component* CreateComponent(uint32_t componentType, int32_t componentSubType = 0);
		
	private:
		/**
		 * \brief 自定义的组件构建器
		 */
		static std::function<Component* (uint32_t, int32_t)> m_CustomCreator;
	};
}
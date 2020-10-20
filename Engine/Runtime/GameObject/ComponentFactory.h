#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Component;
	
	class ComponentFactory final
	{
		friend class GameObject;
	public:
		ComponentFactory() = default;
		~ComponentFactory() = default;

		SINGLETON(ComponentFactory)

		/**
		 * \brief 设置组件构建器
		 * \param creator 组件构建器
		 */
		void SetComponentCreator(const std::function<Component* (uint32_t, int32_t)>& creator)
		{
			m_CustomCreator = creator;
		}
		
	private:
		/**
		 * \brief 根据组件类型创建组件
		 * \param componentType 组件类型
		 * \param param 额外参数
		 * \return 组件指针
		 */
		Component* CreateComponent(uint32_t componentType, int32_t param = 0) const;
		
	private:
		/**
		 * \brief 自定义的组件构建器
		 */
		std::function<Component* (uint32_t, int32_t)> m_CustomCreator;
	};
}
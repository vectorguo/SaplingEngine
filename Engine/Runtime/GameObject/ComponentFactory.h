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
		 * \brief �������������
		 * \param creator ���������
		 */
		void SetComponentCreator(const std::function<Component* (uint32_t, int32_t)>& creator)
		{
			m_CustomCreator = creator;
		}
		
	private:
		/**
		 * \brief ����������ʹ������
		 * \param componentType �������
		 * \param componentSubType ���������
		 * \return ���ָ��
		 */
		Component* CreateComponent(uint32_t componentType, int32_t componentSubType = 0) const;
		
	private:
		/**
		 * \brief �Զ�������������
		 */
		std::function<Component* (uint32_t, int32_t)> m_CustomCreator;
	};
}
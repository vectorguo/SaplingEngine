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
		 * \brief �������������
		 * \param creator ���������
		 */
		static void SetComponentCreator(const std::function<Component* (uint32_t, int32_t)>& creator)
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
		static Component* CreateComponent(uint32_t componentType, int32_t componentSubType = 0);
		
	private:
		/**
		 * \brief �Զ�������������
		 */
		static std::function<Component* (uint32_t, int32_t)> m_CustomCreator;
	};
}
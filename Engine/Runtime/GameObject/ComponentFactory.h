#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class ComponentFactory final
	{
		friend class GameObject;
		
	private:
		/**
		 * \brief ����������ʹ������
		 * \param componentType �������
		 * \param componentSubType ���������
		 * \return ���ָ��
		 */
		static ComponentSptr CreateComponent(uint32_t componentType, int32_t componentSubType = 0);

		/**
		 * \brief ����������ʹ����û��Զ������
		 * \param componentType �������
		 * \param componentSubType ���������
		 * \return ���ָ��
		 */
		static ComponentSptr CreateCustomComponent(uint32_t componentType, int32_t componentSubType = 0);
	};
}
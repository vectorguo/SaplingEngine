#pragma once

#include "GameObject/Component.h"

namespace SaplingEngine
{
	class Renderer : public Component
	{
	public:
		Renderer();

		/*
		 * ��ȡ�������
		 * ÿ����������붨��˺���
		 * GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_Renderer;
		}
	};
}
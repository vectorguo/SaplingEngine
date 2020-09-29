#pragma once

#include "GameObject/Component.h"

namespace SaplingEngine
{
	class Renderer : public Component
	{
	public:
		Renderer();

		/*
		 * 获取组件类型
		 * 每个组件都必须定义此函数
		 * GameObject对于每个ComponentType只能拥有一个Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_Renderer;
		}
	};
}
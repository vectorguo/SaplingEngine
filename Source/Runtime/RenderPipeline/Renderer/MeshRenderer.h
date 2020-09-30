#pragma once

#include "Renderer.h"

namespace SaplingEngine
{
	class Mesh;
	
	class MeshRenderer final : public Renderer
	{
	public:
		MeshRenderer();

		/*
		 * 获取组件类型
		 * 每个组件都必须定义此函数
		 * GameObject对于每个ComponentType只能拥有一个Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_Renderer;
		}

	private:
		Mesh* m_pMesh = nullptr;
	};
}

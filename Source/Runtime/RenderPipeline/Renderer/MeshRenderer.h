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
		 * ��ȡ�������
		 * ÿ����������붨��˺���
		 * GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_Renderer;
		}

	private:
		Mesh* m_pMesh = nullptr;
	};
}

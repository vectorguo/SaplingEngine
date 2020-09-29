#pragma once

#include "Renderer.h"

namespace SaplingEngine
{
	class MeshRenderer : public Renderer
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
	};
}
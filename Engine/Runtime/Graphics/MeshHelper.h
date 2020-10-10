#pragma once

#include "Mesh.h"

namespace SaplingEngine
{
	class MeshHelper
	{
	public:
		/**
		 * \brief 创建Box对应的Mesh
		 * \return Box Mesh
		 */
		static std::unique_ptr<Mesh> CreateBoxMesh();
	};
}
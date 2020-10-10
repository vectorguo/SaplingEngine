#pragma once

#include "Mesh.h"

namespace SaplingEngine
{
	class MeshHelper
	{
	public:
		/**
		 * \brief ����Box��Ӧ��Mesh
		 * \return Box Mesh
		 */
		static std::unique_ptr<Mesh> CreateBoxMesh();
	};
}
#pragma once

#include "MeshResource.h"

namespace SaplingEngine
{
	/**
	 * \brief ����Mesh��Դ
	 * \param meshName Mesh����
	 * \return Resourceָ��
	 */
	IResource* LoadMeshResource(const std::string& meshName);
}

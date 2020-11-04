#pragma once

#include "MeshResource.h"

namespace SaplingEngine
{
	/**
	 * \brief 加载Mesh资源
	 * \param meshName Mesh名称
	 * \return Resource指针
	 */
	IResource* LoadMeshResource(const std::string& meshName);
}

#pragma once

#include "MeshResource.h"

namespace SaplingEngine
{
	/**
	 * \brief 加载Mesh资源
	 * \param path Mesh资源路径
	 * \return Resource指针
	 */
	IResource* LoadMeshResource(const std::string& path);
}

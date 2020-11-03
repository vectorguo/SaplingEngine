#pragma once

#include "MeshResource.h"

namespace SaplingEngine
{
	/**
	 * \brief 加载Mesh资源
	 * \param path Mesh资源路径
	 * \param vertexCount 顶点数量
	 * \param indexCount 索引数量
	 * \return MeshResource指针
	 */
	MeshResource* LoadMeshResource(const std::string& path, uint32_t vertexCount, uint32_t indexCount);
}

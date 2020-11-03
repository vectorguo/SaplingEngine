#pragma once

#include "MeshResource.h"

namespace SaplingEngine
{
	/**
	 * \brief ����Mesh��Դ
	 * \param path Mesh��Դ·��
	 * \param vertexCount ��������
	 * \param indexCount ��������
	 * \return MeshResourceָ��
	 */
	MeshResource* LoadMeshResource(const std::string& path, uint32_t vertexCount, uint32_t indexCount);
}

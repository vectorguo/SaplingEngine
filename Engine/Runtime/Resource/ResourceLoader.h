#pragma once

#include "MeshResource.h"

namespace SaplingEngine
{
	/**
	 * \brief ����Mesh��Դ
	 * \param path Mesh��Դ·��
	 * \return Resourceָ��
	 */
	IResource* LoadMeshResource(const std::string& path);
}

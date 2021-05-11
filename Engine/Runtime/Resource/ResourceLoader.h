#pragma once

namespace SaplingEngine
{
	struct MeshAsset;

	/**
	 * \brief	加载Mesh资源
	 * \param	path		Mesh资源路径
	 * \return	MeshAsset指针
	 */
	MeshAsset* LoadMeshAsset(const std::string& path);
}

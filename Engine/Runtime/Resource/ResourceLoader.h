#pragma once

namespace SaplingEngine
{
	struct MeshAsset;

	/**
	 * \brief	����Mesh��Դ
	 * \param	path		Mesh��Դ·��
	 * \return	MeshAssetָ��
	 */
	MeshAsset* LoadMeshAsset(const std::string& path);
}

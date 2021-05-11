#pragma once

namespace SaplingEngine
{
	struct MeshAsset;

	class Texture2D;
	using Texture2DSptr = SharedPtr<Texture2D>;

	/**
	 * \brief	����Mesh��Դ
	 * \param	path		Mesh��Դ·��
	 * \return	MeshAssetָ��
	 */
	MeshAsset* LoadMeshAsset(const std::string& path);

	/**
	 * \brief	����Texture2D
	 * \param	path		Texture2D��Դ·��
	 * \return	Texture2D����ָ��
	 */
	Texture2DSptr LoadTexture2D(const std::string& path);
}

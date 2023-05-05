#pragma once

#include "Runtime/SaplingEnginePch.h"

namespace SaplingEngine
{
	struct MeshAsset;

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

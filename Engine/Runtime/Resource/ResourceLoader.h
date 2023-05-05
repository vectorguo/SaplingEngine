#pragma once

#include "Runtime/SaplingEnginePch.h"

namespace SaplingEngine
{
	struct MeshAsset;

	/**
	 * \brief	加载Mesh资源
	 * \param	path		Mesh资源路径
	 * \return	MeshAsset指针
	 */
	MeshAsset* LoadMeshAsset(const std::string& path);

	/**
	 * \brief	加载Texture2D
	 * \param	path		Texture2D资源路径
	 * \return	Texture2D智能指针
	 */
	Texture2DSptr LoadTexture2D(const std::string& path);
}

#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class MeshManager
	{
	public:
		/**
		 * \brief	创建Mesh
		 * \param	path		Mesh的资源路径
		 * \return	指向创建好的Mesh的智能指针
		 */
		static MeshSptr CreateMesh(const std::string& path);

		/**
		 * \brief	删除Mesh
		 * \param	meshSptr	要被删除的Mesh的智能指针的引用
		 */
		static void DestroyMesh(MeshSptr&& meshSptr);

		/**
		 * \brief	上传Mesh数据到GPU
		 */
		static void UploadMeshDatas();

	private:
		/**
		 * \brief	数据没有上传到GPU的Mesh
		 */
		static std::vector<Mesh*> unreadyMeshes;
	};
}
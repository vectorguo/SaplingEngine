#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class MeshManager
	{
	public:
		/**
		 * \brief	����Mesh
		 * \param	path		Mesh����Դ·��
		 * \return	ָ�򴴽��õ�Mesh������ָ��
		 */
		static MeshSptr CreateMesh(const std::string& path);

		/**
		 * \brief	ɾ��Mesh
		 * \param	meshSptr	Ҫ��ɾ����Mesh������ָ�������
		 */
		static void DestroyMesh(MeshSptr&& meshSptr);

		/**
		 * \brief	�ϴ�Mesh���ݵ�GPU
		 */
		static void UploadMeshDatas();

	private:
		/**
		 * \brief	����û���ϴ���GPU��Mesh
		 */
		static std::vector<Mesh*> unreadyMeshes;
	};
}
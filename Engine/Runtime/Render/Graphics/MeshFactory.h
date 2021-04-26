#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Mesh;
	class MeshFactory
	{
	public:
		/**
		 * \brief	����Mesh
		 * \param	path		Mesh����Դ·��
		 * \return	ָ�򴴽��õ�Mesh������ָ��
		 */
		static Mesh* CreateMesh(const std::string& path);

		/**
		 * \brief	ɾ��Mesh
		 * \param	pMesh		Ҫ��ɾ����Mesh������ָ�������
		 */
		static void DestroyMesh(Mesh* pMesh);

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
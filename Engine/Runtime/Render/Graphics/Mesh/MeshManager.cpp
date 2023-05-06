#include "Mesh.h"
#include "MeshManager.h"

#include "Runtime/Render/Graphics/GraphicsManager.h"
#include "Runtime/Resource/ResourceManager.h"

namespace SaplingEngine
{
	std::vector<Mesh*>	MeshManager::unreadyMeshes;
	
	/**
	 * \brief	����Mesh
	 * \param	path		Mesh����Դ·��
	 * \return	ָ�򴴽��õ�Mesh��ָ��
	 */
	MeshSptr MeshManager::CreateMesh(const std::string& path)
	{
		auto meshSptr = ResourceManager::Load<Mesh>(path);
		unreadyMeshes.emplace_back(meshSptr.Get());
		return meshSptr;
	}
	
	/**
	 * \brief	ɾ��Mesh
	 * \param	meshSptr	Ҫ��ɾ����Mesh��ָ��
	 */
	void MeshManager::DestroyMesh(MeshSptr&& meshSptr)
	{
		if (!meshSptr->IsReady())
		{
			unreadyMeshes.erase(std::find(unreadyMeshes.begin(), unreadyMeshes.end(), meshSptr.Get()));
		}
	}

	/**
	 * \brief �ϴ�Mesh���ݵ�GPU
	 */
	void MeshManager::UploadMeshDatas()
	{
		if (!unreadyMeshes.empty())
		{
			for (auto iter = unreadyMeshes.begin(); iter != unreadyMeshes.end(); ++iter)
			{
				auto* pMesh = *iter;
				pMesh->m_IsReady = true;

				//�������㻺����������
				const auto vertexSize = static_cast<uint32_t>(pMesh->GetVertexCount() * sizeof(VertexData));
				pMesh->m_VertexBufferOnGpu = GraphicsManager::CreateDefaultBufferAndUploadData(pMesh->GetVertexDatas(), vertexSize);
				pMesh->m_pVertexBufferView = new D3D12_VERTEX_BUFFER_VIEW{ pMesh->m_VertexBufferOnGpu->GetGPUVirtualAddress(), vertexSize, sizeof(VertexData) };

				//��������������������
				const auto indexSize = static_cast<uint32_t>(pMesh->GetIndexCount() * sizeof(uint16_t));
				pMesh->m_IndexBufferOnGpu = GraphicsManager::CreateDefaultBufferAndUploadData(pMesh->GetIndices(), indexSize);
				pMesh->m_pIndexBufferView = new D3D12_INDEX_BUFFER_VIEW{ pMesh->m_IndexBufferOnGpu->GetGPUVirtualAddress(), indexSize, DXGI_FORMAT_R16_UINT };
			}
			unreadyMeshes.clear();
		}
	}
}
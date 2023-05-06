#include "Mesh.h"
#include "MeshManager.h"

#include "Runtime/Render/Graphics/GraphicsManager.h"
#include "Runtime/Resource/ResourceManager.h"

namespace SaplingEngine
{
	std::vector<Mesh*>	MeshManager::unreadyMeshes;
	
	/**
	 * \brief	创建Mesh
	 * \param	path		Mesh的资源路径
	 * \return	指向创建好的Mesh的指针
	 */
	MeshSptr MeshManager::CreateMesh(const std::string& path)
	{
		auto meshSptr = ResourceManager::Load<Mesh>(path);
		unreadyMeshes.emplace_back(meshSptr.Get());
		return meshSptr;
	}
	
	/**
	 * \brief	删除Mesh
	 * \param	meshSptr	要被删除的Mesh的指针
	 */
	void MeshManager::DestroyMesh(MeshSptr&& meshSptr)
	{
		if (!meshSptr->IsReady())
		{
			unreadyMeshes.erase(std::find(unreadyMeshes.begin(), unreadyMeshes.end(), meshSptr.Get()));
		}
	}

	/**
	 * \brief 上传Mesh数据到GPU
	 */
	void MeshManager::UploadMeshDatas()
	{
		if (!unreadyMeshes.empty())
		{
			for (auto iter = unreadyMeshes.begin(); iter != unreadyMeshes.end(); ++iter)
			{
				auto* pMesh = *iter;
				pMesh->m_IsReady = true;

				//创建顶点缓冲区描述符
				const auto vertexSize = static_cast<uint32_t>(pMesh->GetVertexCount() * sizeof(VertexData));
				pMesh->m_VertexBufferOnGpu = GraphicsManager::CreateDefaultBufferAndUploadData(pMesh->GetVertexDatas(), vertexSize);
				pMesh->m_pVertexBufferView = new D3D12_VERTEX_BUFFER_VIEW{ pMesh->m_VertexBufferOnGpu->GetGPUVirtualAddress(), vertexSize, sizeof(VertexData) };

				//创建索引缓冲区描述符
				const auto indexSize = static_cast<uint32_t>(pMesh->GetIndexCount() * sizeof(uint16_t));
				pMesh->m_IndexBufferOnGpu = GraphicsManager::CreateDefaultBufferAndUploadData(pMesh->GetIndices(), indexSize);
				pMesh->m_pIndexBufferView = new D3D12_INDEX_BUFFER_VIEW{ pMesh->m_IndexBufferOnGpu->GetGPUVirtualAddress(), indexSize, DXGI_FORMAT_R16_UINT };
			}
			unreadyMeshes.clear();
		}
	}
}
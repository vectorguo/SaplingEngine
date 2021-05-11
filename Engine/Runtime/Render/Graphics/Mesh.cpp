#include "Render/Graphics/DirectX12/Dx12GraphicsManager.h"
#include "Render/Graphics/Mesh.h"

namespace SaplingEngine
{
	Mesh::Mesh(MeshAsset* pAsset) : m_AssetPtr(pAsset)
	{
	}

	Mesh::~Mesh()
	{
		delete m_AssetPtr;
		delete m_pVertexBufferView;
		delete m_pIndexBufferView;
	}
}

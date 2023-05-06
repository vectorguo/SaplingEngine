#include "Mesh.h"

#include "Runtime/Render/Graphics/GraphicsManager.h"

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

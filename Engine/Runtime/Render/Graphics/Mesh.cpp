#include "DirectX12/Dx12GraphicsManager.h"

#include "Mesh.h"
#include "MeshFactory.h"

namespace SaplingEngine
{
	Mesh::Mesh()
	{
	}

	Mesh::~Mesh()
	{
		delete m_pVertexBufferView;
		delete m_pIndexBufferView;
	}
}

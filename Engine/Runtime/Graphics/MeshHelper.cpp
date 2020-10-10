#include "MeshHelper.h"

namespace SaplingEngine
{
	/**
	 * \brief 创建Box对应的Mesh
	 * \return Box Mesh
	 */
	std::unique_ptr<Mesh> MeshHelper::CreateBoxMesh()
	{
		std::vector<VertexData> vertices =
		{
			VertexData({ XMFLOAT3(-1.0f, -1.0f, -1.0), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(Colors::White),	XMFLOAT2(0, 0), XMFLOAT2(0, 0) }),
			VertexData({ XMFLOAT3(-1.0f, +1.0f, -1.0), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(Colors::Black),	XMFLOAT2(0, 0), XMFLOAT2(0, 0) }),
			VertexData({ XMFLOAT3(+1.0f, +1.0f, -1.0), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(Colors::Red),	XMFLOAT2(0, 0), XMFLOAT2(0, 0) }),
			VertexData({ XMFLOAT3(+1.0f, -1.0f, -1.0), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(Colors::Green),	XMFLOAT2(0, 0), XMFLOAT2(0, 0) }),
			VertexData({ XMFLOAT3(-1.0f, -1.0f, +1.0), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(Colors::Blue),	XMFLOAT2(0, 0), XMFLOAT2(0, 0) }),
			VertexData({ XMFLOAT3(-1.0f, +1.0f, +1.0), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(Colors::Yellow),	XMFLOAT2(0, 0), XMFLOAT2(0, 0) }),
			VertexData({ XMFLOAT3(+1.0f, +1.0f, +1.0), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(Colors::Cyan),	XMFLOAT2(0, 0), XMFLOAT2(0, 0) }),
			VertexData({ XMFLOAT3(+1.0f, -1.0f, +1.0), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(Colors::Magenta),XMFLOAT2(0, 0), XMFLOAT2(0, 0) })
		};

		std::vector<std::uint16_t> indices =
		{
			// front face
			0, 1, 2,
			0, 2, 3,

			// back face
			4, 6, 5,
			4, 7, 6,

			// left face
			4, 5, 1,
			4, 1, 0,

			// right face
			3, 2, 6,
			3, 6, 7,

			// top face
			1, 5, 6,
			1, 6, 2,

			// bottom face
			4, 0, 3,
			4, 3, 7
		};
		
		auto pMesh = std::make_unique<Mesh>();
		pMesh->SetVertexDatas(std::move(vertices));
		pMesh->SetIndices(std::move(indices));
		return pMesh;
	}
}

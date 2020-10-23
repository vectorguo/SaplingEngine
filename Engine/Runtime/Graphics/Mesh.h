#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	struct VertexData
	{
		XMFLOAT3 PositionOS;		//Vertex Position in Object Space
		XMFLOAT3 NormalOS;			//Normal in Object Space
		XMFLOAT4 Color;				//Vertex Color
		XMFLOAT2 UV0;				//UV0
		XMFLOAT2 UV1;				//UV1
	};

	/**
	 * \brief Mesh基本类型
	 */
	enum class EMeshPrimitiveType
	{
		Cube,
	};
	
	class Mesh
	{
	public:
		Mesh();
		~Mesh();
		
		Mesh(const Mesh&) = delete;
		Mesh(Mesh&&) = delete;
		Mesh& operator=(const Mesh&) = delete;
		Mesh& operator=(Mesh&&) = delete;

		/**
		 * \brief 顶点和索引数据是否已经上传到默认缓冲区
		 * \return 顶点和索引数据是否已经上传到默认缓冲区
		 */
		bool IsReady() const
		{
			return m_IsReady;
		}
		
		/**
		 * \brief 获取顶点缓冲区描述符
		 * \return 顶点缓冲区描述符
		 */
		const D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferView() const
		{
			return m_pVertexBufferView;
		}

		/**
		 * \brief 获取索引缓冲区描述符
		 * \return 索引缓冲区描述符
		 */
		const D3D12_INDEX_BUFFER_VIEW* GetIndexBufferView() const
		{
			return m_pIndexBufferView;
		}
		
		const std::vector<VertexData>& GetVertexDatas() const
		{
			return m_VertexDatas;
		}

		std::vector<VertexData>& GetVertexDatas()
		{
			return m_VertexDatas;
		}

		uint32_t GetVertexCount() const
		{
			return static_cast<uint32_t>(m_VertexDatas.size());
		}

		void SetVertexDatas(const std::vector<VertexData>& vertices)
		{
			m_VertexDatas = vertices;
		}

		void SetVertexDatas(std::vector<VertexData>&& vertices)
		{
			m_VertexDatas = std::move(vertices);
		}

		void SetVertexDatas(VertexData* pVertices, const uint32_t size)
		{
			m_VertexDatas.reserve(size);
			for (uint32_t i = 0; i < size; ++i)
			{
				m_VertexDatas.emplace_back(*(pVertices + i));
			}
		}
		
		const std::vector<uint16_t>& GetIndices() const
		{
			return m_Indices;
		}

		std::vector<uint16_t>& GetIndices()
		{
			return m_Indices;
		}

		uint32_t GetIndexCount() const
		{
			return static_cast<uint32_t>(m_Indices.size());
		}

		void SetIndices(const std::vector<uint16_t>& indices)
		{
			m_Indices = indices;
		}

		void SetIndices(std::vector<uint16_t>&& indices)
		{
			m_Indices = std::move(indices);
		}

		void SetIndices(uint16_t* pIndices, const uint32_t size)
		{
			m_Indices.reserve(size);
			for (uint32_t i = 0; i < size; ++i)
			{
				m_Indices.emplace_back(*(pIndices + i));
			}
		}

	public:
		/**
		 * \brief 上传Mesh数据到GPU
		 */
		static void UploadMeshDatas();
		
		/**
		 * \brief 创建基本类型的mesh
		 * \param type mesh类型
		 * \return mesh指针
		 */
		static Mesh* CreatePrimitive(EMeshPrimitiveType type);
		
	private:
		
		/**
		 * \brief 数据没有上传到GPU的Mesh
		 */
		static std::vector<Mesh*> m_NotUploadedMeshes;
		
		/**
		 * \brief 顶点和索引数据是否已经上传到默认缓冲区
		 */
		bool m_IsReady;
		
		/**
		 * \brief 顶点数据
		 */
		std::vector<VertexData> m_VertexDatas;
		
		/**
		 * \brief 索引数据
		 */
		std::vector<uint16_t> m_Indices;

		ComPtr<ID3D12Resource> m_VertexBufferOnGpu = nullptr;
		D3D12_VERTEX_BUFFER_VIEW* m_pVertexBufferView = nullptr;
		
		ComPtr<ID3D12Resource> m_IndexBufferOnGpu = nullptr;
		D3D12_INDEX_BUFFER_VIEW* m_pIndexBufferView = nullptr;
	};
	
	// class Mesh
	// {
	// public:
	// 	Mesh() = default;
	//
	// 	const std::vector<Vector3>& GetVertices() const
	// 	{
	// 		return m_Vertices;
	// 	}
	//
	// 	std::vector<Vector3>& GetVertices()
	// 	{
	// 		return m_Vertices;
	// 	}
	// 	
	// 	void SetVertices(const std::vector<Vector3>& vertices)
	// 	{
	// 		m_Vertices = vertices;
	// 	}
	// 	
	// 	void SetVertices(std::vector<Vector3>&& vertices)
	// 	{
	// 		m_Vertices = std::move(vertices);
	// 	}
	// 	
	// 	void SetVertices(Vector3* pVertices, const uint32_t size)
	// 	{
	// 		m_Vertices.reserve(size);
	// 		for (uint32_t i = 0; i < size; ++i)
	// 		{
	// 			m_Vertices.emplace_back(*(pVertices + i));
	// 		}
	// 	}
	//
	// 	const std::vector<Vector3>& GetNormals() const
	// 	{
	// 		return m_Normals;
	// 	}
	//
	// 	std::vector<Vector3>& GetNormals()
	// 	{
	// 		return m_Normals;
	// 	}
	// 	
	// 	void SetNormals(const std::vector<Vector3>& normals)
	// 	{
	// 		m_Normals = normals;
	// 	}
	// 	
	// 	void SetNormals(std::vector<Vector3>&& normals)
	// 	{
	// 		m_Normals = std::move(normals);
	// 	}
	// 	
	// 	void SetNormals(Vector3* pNormals, const uint32_t size)
	// 	{
	// 		m_Normals.reserve(size);
	// 		for (uint32_t i = 0; i < size; ++i)
	// 		{
	// 			m_Normals.emplace_back(*(pNormals + i));
	// 		}
	// 	}
	//
	// 	const std::vector<Color>& GetColors() const
	// 	{
	// 		return m_Colors;
	// 	}
	//
	// 	std::vector<Color>& GetColors()
	// 	{
	// 		return m_Colors;
	// 	}
	// 	
	// 	void SetColors(const std::vector<Color>& colors)
	// 	{
	// 		m_Colors = colors;
	// 	}
	// 	
	// 	void SetColors(std::vector<Color>&& colors)
	// 	{
	// 		m_Colors = std::move(colors);
	// 	}
	// 	
	// 	void SetColors(Color* pColors, const uint32_t size)
	// 	{
	// 		m_Colors.reserve(size);
	// 		for (uint32_t i = 0; i < size; ++i)
	// 		{
	// 			m_Colors.emplace_back(*(pColors + i));
	// 		}
	// 	}
	//
	// 	const std::vector<Vector2>& GetUvs(const int32_t channel) const
	// 	{
	// 		return m_Uv0;
	// 	}
	//
	// 	std::vector<Vector2>& GetUvs(const int32_t channel)
	// 	{
	// 		return m_Uv0;
	// 	}
	// 	
	// 	void SetUvs(const int32_t channel, const std::vector<Vector2>& uvs)
	// 	{
	// 		if (channel == 0)
	// 		{
	// 			m_Uv0 = uvs;
	// 		}
	// 	}
	// 	
	// 	void SetUvs(const int32_t channel, std::vector<Vector2>&& uvs)
	// 	{
	// 		if (channel == 0)
	// 		{
	// 			m_Uv0 = std::move(uvs);
	// 		}
	// 	}
	// 	
	// 	void SetUvs(const int32_t channel, Vector2* pUvs, const uint32_t size)
	// 	{
	// 		if (channel == 0)
	// 		{
	// 			m_Uv0.reserve(size);
	// 			for (uint32_t i = 0; i < size; ++i)
	// 			{
	// 				m_Uv0.emplace_back(*(pUvs + i));
	// 			}
	// 		}
	// 	}
	//
	// 	const std::vector<uint32_t>& GetIndices() const
	// 	{
	// 		return m_Indices;
	// 	}
	//
	// 	std::vector<uint32_t>& GetIndices()
	// 	{
	// 		return m_Indices;
	// 	}
	// 	
	// 	void SetIndices(const std::vector<uint32_t>& indices)
	// 	{
	// 		m_Indices = indices;
	// 	}
	// 	
	// 	void SetIndices(std::vector<uint32_t>&& indices)
	// 	{
	// 		m_Indices = std::move(indices);
	// 	}
	// 	
	// 	void SetIndices(uint32_t* pIndices, const uint32_t size)
	// 	{
	// 		m_Indices.reserve(size);
	// 		for (uint32_t i = 0; i < size; ++i)
	// 		{
	// 			m_Indices.emplace_back(*(pIndices + i));
	// 		}
	// 	}
	// 	
	// private:
	// 	std::vector<Vector3> m_Vertices;
	// 	std::vector<Vector3> m_Normals;
	// 	std::vector<Color> m_Colors;
	// 	std::vector<Vector2> m_Uv0;
	//
	// 	std::vector<uint32_t> m_Indices;
	// };
}
#pragma once

#include "IResource.h"
#include "Render/Graphics/VertexData.h"

namespace SaplingEngine
{
	class MeshResource final : public IResource
	{
	public:
		explicit MeshResource()
			: IResource()
		{
		}

		~MeshResource() override = default;

		/*
		 * 禁止拷贝和移动
		 */
		MeshResource(const MeshResource&) = delete;
		MeshResource(MeshResource&&) = delete;
		MeshResource& operator= (const MeshResource&) = delete;
		MeshResource& operator= (MeshResource&&) = delete;

		/**
		 * \brief 获取资源类型
		 * \return 资源类型
		 */
		static EResourceType GetResourceType() 
		{
			return EResourceType::Mesh;
		}
		
		/**
		 * \brief 获取顶点数据
		 * \return 顶点数据
		 */
		const std::vector<VertexData>& GetVertexDatas() const
		{
			return m_VertexDatas;
		}

		/**
		 * \brief 设置顶点数据
		 * \param vertexDatas 顶点数据
		 */
		void SetVertexDatas(std::vector<VertexData>&& vertexDatas)
		{
			m_VertexDatas = std::move(vertexDatas);
		}

		/**
		 * \brief 获取顶点数量
		 * \return 顶点数量
		 */
		uint32_t GetVertexCount() const
		{
			return static_cast<uint32_t>(m_VertexDatas.size());
		}
		
		/**
		 * \brief 获取索引数据
		 * \return 索引数据
		 */
		const std::vector<uint16_t>& GetIndices() const
		{
			return m_Indices;
		}

		/**
		 * \brief 设置索引数据
		 * \param indices 索引数据
		 */
		void SetIndices(std::vector<uint16_t>&& indices)
		{
			m_Indices = std::move(indices);
		}
		
		/**
		 * \brief 获取索引数量
		 * \return 索引数量
		 */
		uint32_t GetIndexCount() const
		{
			return static_cast<uint32_t>(m_Indices.size());
		}
		
	private:
		/**
		 * \brief 顶点数据
		 */
		std::vector<VertexData> m_VertexDatas;

		/**
		 * \brief 索引数据
		 */
		std::vector<uint16_t> m_Indices;
	};
}

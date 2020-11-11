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
		 * ��ֹ�������ƶ�
		 */
		MeshResource(const MeshResource&) = delete;
		MeshResource(MeshResource&&) = delete;
		MeshResource& operator= (const MeshResource&) = delete;
		MeshResource& operator= (MeshResource&&) = delete;

		/**
		 * \brief ��ȡ��Դ����
		 * \return ��Դ����
		 */
		static EResourceType GetResourceType() 
		{
			return EResourceType::Mesh;
		}
		
		/**
		 * \brief ��ȡ��������
		 * \return ��������
		 */
		const std::vector<VertexData>& GetVertexDatas() const
		{
			return m_VertexDatas;
		}

		/**
		 * \brief ���ö�������
		 * \param vertexDatas ��������
		 */
		void SetVertexDatas(std::vector<VertexData>&& vertexDatas)
		{
			m_VertexDatas = std::move(vertexDatas);
		}

		/**
		 * \brief ��ȡ��������
		 * \return ��������
		 */
		uint32_t GetVertexCount() const
		{
			return static_cast<uint32_t>(m_VertexDatas.size());
		}
		
		/**
		 * \brief ��ȡ��������
		 * \return ��������
		 */
		const std::vector<uint16_t>& GetIndices() const
		{
			return m_Indices;
		}

		/**
		 * \brief ������������
		 * \param indices ��������
		 */
		void SetIndices(std::vector<uint16_t>&& indices)
		{
			m_Indices = std::move(indices);
		}
		
		/**
		 * \brief ��ȡ��������
		 * \return ��������
		 */
		uint32_t GetIndexCount() const
		{
			return static_cast<uint32_t>(m_Indices.size());
		}
		
	private:
		/**
		 * \brief ��������
		 */
		std::vector<VertexData> m_VertexDatas;

		/**
		 * \brief ��������
		 */
		std::vector<uint16_t> m_Indices;
	};
}

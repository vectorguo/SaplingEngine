#pragma once

#include "Renderer.h"

namespace SaplingEngine
{
	class Material;
	class Mesh;
	
	class MeshRenderer final : public Renderer
	{
	public:
		MeshRenderer();
		~MeshRenderer() override;

		/*
		 * ��ȡ�������
		 * ÿ����������붨��˺���
		 * GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_Renderer;
		}

		/**
		 * \brief ��ȡMaterial
		 * \return Material
		 */
		Material* GetMaterial() const
		{
			return m_pMaterial;
		}
		
		/**
		 * \brief ��ȡMesh
		 * \return Mesh
		 */
		Mesh* GetMesh() const
		{
			return m_pMesh;
		}
		
		/**
		 * \brief �����л�
		 * \param pNode ���ýڵ�ָ��
		 * \return �����л��Ƿ�ɹ�
		 */
		bool Deserialize(const XmlNode* pNode) override;
		
	private:
		Material* m_pMaterial = nullptr;
		Mesh* m_pMesh = nullptr;
	};
}

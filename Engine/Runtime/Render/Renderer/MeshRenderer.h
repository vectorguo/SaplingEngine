#pragma once

#include "Renderer.h"

namespace SaplingEngine
{	
	class MeshRenderer final : public Renderer
	{
	public:
		MeshRenderer();
		~MeshRenderer() override;

		/*
		 * ��ֹ�������ƶ�
		 */
		MeshRenderer(const MeshRenderer&) = delete;
		MeshRenderer(MeshRenderer&&) = delete;
		MeshRenderer& operator= (const MeshRenderer&) = delete;
		MeshRenderer& operator= (MeshRenderer&&) = delete;
		
		/*
		 * ��ȡ�������
		 * ÿ����������붨��˺���
		 * GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component
		 */
		static constexpr uint32_t GetStaticComponentType()
		{
			return ComponentType_Renderer;
		}

		/**
		 * \brief	����Mesh
		 * \param	shaderName	Shader����
		 * \return	����
		 */
		MaterialSptr& SetMaterial(const std::string& shaderName) override;

		/**
		 * \brief	����Mesh
		 * \param	path		Mesh��Դ·��
		 */
		void SetMesh(const std::string& path) override;

	private:
		/**
		 * \brief �����л�
		 * \param pNode ���ýڵ�ָ��
		 * \return �����л��Ƿ�ɹ�
		 */
		bool Deserialize(const XmlNode* pNode) override;
	};
}

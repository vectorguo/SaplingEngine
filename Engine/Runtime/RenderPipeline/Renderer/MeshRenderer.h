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
		 * 获取组件类型
		 * 每个组件都必须定义此函数
		 * GameObject对于每个ComponentType只能拥有一个Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_Renderer;
		}

		/**
		 * \brief 获取Material
		 * \return Material
		 */
		Material* GetMaterial() const
		{
			return m_pMaterial;
		}
		
		/**
		 * \brief 获取Mesh
		 * \return Mesh
		 */
		Mesh* GetMesh() const
		{
			return m_pMesh;
		}
		
		/**
		 * \brief 反序列化
		 * \param pNode 配置节点指针
		 * \return 反序列化是否成功
		 */
		bool Deserialize(const XmlNode* pNode) override;
		
	private:
		Material* m_pMaterial = nullptr;
		Mesh* m_pMesh = nullptr;
	};
}

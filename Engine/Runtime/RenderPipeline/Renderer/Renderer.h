#pragma once

#include "GameObject/Component.h"

namespace SaplingEngine
{
	class Material;
	class Mesh;
	
	class Renderer : public Component
	{
	public:
		Renderer();
		~Renderer() override;

		/*
		 * ��ֹ�������ƶ�
		 */
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator= (const Renderer&) = delete;
		Renderer& operator= (Renderer&&) = delete;
		
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
		 * \brief ��ȡ����������������
		 * \return ����������������
		 */
		int32_t GetConstantBufferIndex() const
		{
			return m_ConstantBufferIndex;
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

	protected:
		void Start() override;
		void OnDestroy() override;

	protected:
		/**
		 * \brief ����
		 */
		Material* m_pMaterial = nullptr;

		/**
		 * \brief Mesh
		 */
		Mesh* m_pMesh = nullptr;
		
	private:
		/**
		 * \brief ����������������
		 */
		int32_t m_ConstantBufferIndex = -1;
	};
}
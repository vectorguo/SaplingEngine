#pragma once

#include "GameObject/Component.h"
#include "Render/Graphics/ConstantBufferData.h"
#include "Render/Graphics/Material.h"
#include "Render/Graphics/Mesh.h"
#include "Render/Graphics/DirectX12/Dx12CBufferManager.h"

namespace SaplingEngine
{
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

		uint32_t GetCommonOcbIndex() const
		{
			return m_OcbIndex;
		}

		uint32_t GetSpecialOcbIndex() const
		{
			return m_OcbIndex + CBufferManager::ConstantBufferElementCount;
		}
		
		/**
		 * \brief ��ȡObject�����ⳣ������������ָ��
		 * \return Object�����ⳣ������������ָ��
		 */
		ISpecialOcbData* GetSpecialOcbData() const
		{
			return m_pSpecialOcbData;
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

		/**
		 * \brief Object�����ⳣ������������
		 */
		ISpecialOcbData* m_pSpecialOcbData = nullptr;
		
	private:
		/**
		 * \brief ����������������
		 */
		uint32_t m_OcbIndex = 0;
	};
}

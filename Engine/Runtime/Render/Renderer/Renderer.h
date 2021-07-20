#pragma once

#include "GameObject/Component.h"
#include "Render/Graphics/ConstantBufferData.h"
#include "Render/Graphics/Material.h"
#include "Render/Graphics/Mesh.h"

namespace SaplingEngine
{
	class Renderer : public Component
	{
		friend class RenderPipeline;

	public:
		Renderer();
		~Renderer() override;

		/**
		 * ��ֹ�������ƶ�
		 */
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator= (const Renderer&) = delete;
		Renderer& operator= (Renderer&&) = delete;
		
		/**
		 * \brief	��ȡ������͡�
		 *			ÿ����������붨��˺�����
		 *			GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component��
		 */
		static constexpr uint32_t GetStaticComponentType()
		{
			return ComponentType_Renderer;
		}

		/**
		 * \brief	��ȡMaterial
		 * \return	Material
		 */
		Material* GetMaterial() const
		{
			return m_MaterialSptr.Get();
		}

		/**
		 * \brief	��ȡMaterial����ָ��
		 * \return	Material
		 */
		const MaterialSptr& GetMaterialSptr() const
		{
			return m_MaterialSptr;
		}

		/**
		 * \brief	����Mesh
		 * \param	shaderName	Shader����
		 * \return	����
		 */
		MaterialSptr& SetMaterial(const std::string& shaderName);

		/**
		 * \brief	��ȡMesh
		 * \return	Mesh
		 */
		Mesh* GetMesh() const
		{
			return m_MeshSptr.Get();
		}

		/**
		 * \brief	����Mesh
		 * \param	path		Mesh��Դ·��
		 */
		virtual void SetMesh(const std::string& path) = 0;

		/**
		 * \brief	��ȡ���峣������������
		 * \return	���峣������������
		 */
		int32_t GetCbvIndex() const
		{
			return m_CbvIndex;
		}

		/**
		 * \brief	��ȡͨ�����ݶ�Ӧ�ĳ���������������
		 * \return	ͨ�����ݶ�Ӧ�ĳ���������������
		 */
		D3D12_GPU_VIRTUAL_ADDRESS GetCommonCbAddress() const
		{
			return m_CommonCbAddress;
		}

		/**
		 * \brief	��ȡ�������ݶ�Ӧ�ĳ�����������ַ
		 * \return	�������ݶ�Ӧ�ĳ�����������ַ
		 */
		D3D12_GPU_VIRTUAL_ADDRESS GetSpecialCbAddress() const
		{
			return m_SpecialCbAddress;
		}
		
		/**
		 * \brief	���SpecialOcbData
		 */
		void* FillSpecialOcbData(size_t& size, Material* pMaterial) const
		{
			return m_FillSpecialOcbDataHandler(size, pMaterial);
		}

	protected:
		void OnEnable() override;
		void OnDisable() override;
		void OnDestroy() override;

	protected:
		/**
		 * \brief	����
		 */
		MaterialSptr m_MaterialSptr;

		/**
		 * \brief	Mesh
		 */
		MeshSptr m_MeshSptr;

		/**
		 * \brief	Object�����ⳣ������������Fill�ص�
		 */
		std::function<void*(size_t&, Material*)> m_FillSpecialOcbDataHandler;
		
	private:
		/**
		 * \brief	����������������������
		 */
		int32_t m_CbvHeapIndex = -1;

		/**
		 * \brief	��������������������
		 */
		int32_t m_CbvIndex = -1;

		/**
		 * \brief	ͨ�����ݳ�����������ַ
		 */
		D3D12_GPU_VIRTUAL_ADDRESS m_CommonCbAddress{};

		/**
		 * \brief	�������ݳ�����������ַ
		 */
		D3D12_GPU_VIRTUAL_ADDRESS m_SpecialCbAddress{};
	};
}

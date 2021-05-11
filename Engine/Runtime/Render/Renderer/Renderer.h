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
		virtual MaterialSptr& SetMaterial(const std::string& shaderName) = 0;

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
		uint32_t GetCbvIndex() const
		{
			return m_CbvIndex;
		}

		/**
		 * \brief	��ȡͨ�����ݶ�Ӧ�ĳ���������������
		 * \return	ͨ�����ݶ�Ӧ�ĳ���������������
		 */
		D3D12_GPU_DESCRIPTOR_HANDLE GetCommonCbvDescriptor() const
		{
			return m_CommonCbvDescriptor;
		}

		/**
		 * \brief	��ȡ�������ݶ�Ӧ�ĳ���������������
		 * \return	�������ݶ�Ӧ�ĳ���������������
		 */
		D3D12_GPU_DESCRIPTOR_HANDLE GetSpecialCbvDescriptor() const
		{
			return m_SpecialCbvDescriptor;
		}
		
		/**
		 * \brief	���SpecialOcbData
		 */
		void* FillSpecialOcbData(size_t& size, Material* pMaterial) const
		{
			return m_FillSpecialOcbDataHandler(size, pMaterial);
		}

	protected:
		void Start() override;
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
		 * \brief	����������������
		 */
		uint32_t m_CbvIndex = 0;

		/**
		 * \brief	ͨ�����ݳ�����������������
		 */
		D3D12_GPU_DESCRIPTOR_HANDLE m_CommonCbvDescriptor{};

		/**
		 * \brief	�������ݳ�����������������
		 */
		D3D12_GPU_DESCRIPTOR_HANDLE m_SpecialCbvDescriptor{};
	};
}

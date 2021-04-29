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
		 * 禁止拷贝和移动
		 */
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator= (const Renderer&) = delete;
		Renderer& operator= (Renderer&&) = delete;
		
		/**
		 * \brief	获取组件类型。
		 *			每个组件都必须定义此函数。
		 *			GameObject对于每个ComponentType只能拥有一个Component。
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_Renderer;
		}

		/**
		 * \brief	获取Material
		 * \return	Material
		 */
		Material* GetMaterial() const
		{
			return m_pMaterial;
		}

		/**
		 * \brief	获取Mesh
		 * \return	Mesh
		 */
		Mesh* GetMesh() const
		{
			return m_pMesh;
		}

		/**
		 * \brief	获取物体常量缓冲区索引
		 * \return	物体常量缓冲区索引
		 */
		uint32_t GetCbvIndex() const
		{
			return m_CbvIndex;
		}

		/**
		 * \brief	获取通用数据对应的常量缓冲区描述符
		 * \return	通用数据对应的常量缓冲区描述符
		 */
		D3D12_GPU_DESCRIPTOR_HANDLE GetCommonCbvDescriptor() const
		{
			return m_CommonCbvDescriptor;
		}

		/**
		 * \brief	获取特殊数据对应的常量缓冲区描述符
		 * \return	特殊数据对应的常量缓冲区描述符
		 */
		D3D12_GPU_DESCRIPTOR_HANDLE GetSpecialCbvDescriptor() const
		{
			return m_SpecialCbvDescriptor;
		}
		
		/**
		 * \brief	填充SpecialOcbData
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
		 * \brief	材质
		 */
		Material* m_pMaterial = nullptr;

		/**
		 * \brief	Mesh
		 */
		Mesh* m_pMesh = nullptr;

		/**
		 * \brief	Object的特殊常量缓冲区数据Fill回调
		 */
		std::function<void*(size_t&, Material*)> m_FillSpecialOcbDataHandler;
		
	private:
		/**
		 * \brief	常量缓冲区的索引
		 */
		uint32_t m_CbvIndex = 0;

		/**
		 * \brief	通用数据常量缓冲区的描述符
		 */
		D3D12_GPU_DESCRIPTOR_HANDLE m_CommonCbvDescriptor{};

		/**
		 * \brief	特殊数据常量缓冲区的描述符
		 */
		D3D12_GPU_DESCRIPTOR_HANDLE m_SpecialCbvDescriptor{};
	};
}

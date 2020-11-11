#pragma once

#include "GameObject/Component.h"

namespace SaplingEngine
{
	class IConstantBufferData;
	class Material;
	class Mesh;
	
	class Renderer : public Component
	{
	public:
		Renderer();
		~Renderer() override;

		/*
		 * 禁止拷贝和移动
		 */
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator= (const Renderer&) = delete;
		Renderer& operator= (Renderer&&) = delete;
		
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

		uint32_t GetCommonOcbIndex() const
		{
			return m_OcbIndex;
		}

		uint32_t GetSpecialOcbIndex() const
		{
			return m_OcbIndex + CbElementCount;
		}
		
		/**
		 * \brief 获取Object的特殊常量缓冲区数据指针
		 * \return Object的特殊常量缓冲区数据指针
		 */
		IConstantBufferData* GetSpecialOcbData() const
		{
			return m_pSpecialOcbData;
		}

	protected:
		void Start() override;
		void OnDestroy() override;

	protected:
		/**
		 * \brief 材质
		 */
		Material* m_pMaterial = nullptr;

		/**
		 * \brief Mesh
		 */
		Mesh* m_pMesh = nullptr;

		/**
		 * \brief Object的特殊常量缓冲区数据
		 */
		IConstantBufferData* m_pSpecialOcbData = nullptr;
		
	private:
		/**
		 * \brief 常量缓冲区的索引
		 */
		uint32_t m_OcbIndex = 0;
	};
}
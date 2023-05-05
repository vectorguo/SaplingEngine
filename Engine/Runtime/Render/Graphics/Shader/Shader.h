#pragma once

#include "Runtime/SaplingEnginePch.h"

namespace SaplingEngine
{
	class Shader
	{
	public:
		explicit Shader(const XmlNode* pShaderNode);
		~Shader();
		
		Shader(const Shader&) = delete;
		Shader(Shader&&) = delete;
		Shader& operator=(const Shader&) = delete;
		Shader& operator=(Shader&&) = delete;

		/**
		 * \brief 获取Shader名称
		 * \return Shader名称
		 */
		const std::string& GetName() const
		{
			return m_Name;
		}

		/**
		 * \brief 获取Shader名称对应的Hash值
		 * \return Shader名称对应的Hash值
		 */
		size_t GetHashValue() const
		{
			return m_HashValue;
		}

		/**
		 * \brief 获取Shader对应的物体常量缓冲区类型
		 * \return Shader对应的物体常量缓冲区类型
		 */
		uint8_t GetOcbType() const
		{
			return m_OcbType;
		}

		uint32_t GetTextureCount() const
		{
			return m_TextureCount;
		}

		uint8_t* GetVsBufferPoint() const
		{
			return static_cast<BYTE*>(m_VsShader->GetBufferPointer());
		}

		uint64_t GetVsBufferSize() const
		{
			return m_VsShader->GetBufferSize();
		}

		uint8_t* GetPsBufferPoint() const
		{
			return static_cast<BYTE*>(m_PsShader->GetBufferPointer());
		}

		uint64_t GetPsBufferSize() const
		{
			return m_PsShader->GetBufferSize();
		}

		/**
		 * \brief 获取Shader的输入布局
		 * \return Shader的输入布局
		 */
		const std::vector<D3D12_INPUT_ELEMENT_DESC>* GetInputLayout() const
		{
			return &m_InputLayout;
		}
		
	private:
		/**
		 * \brief Shader名称
		 */
		std::string m_Name;

		/**
		 * \brief Shader名称对应的HashValue
		 */
		size_t m_HashValue;

		/**
		 * \brief Shader类型，用于区分不同shader对应的SpecialCBuffer
		 */
		uint8_t m_OcbType;

		/**
		 * \brief Shader所需的贴图数量
		 */
		uint32_t m_TextureCount;

		ComPtr<ID3DBlob> m_VsShader;
		ComPtr<ID3DBlob> m_PsShader;

		/**
		 * \brief Shader输入布局
		 */
		std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;
	};
}
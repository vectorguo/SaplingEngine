#pragma once

#include "SaplingEnginePch.h"

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
		std::string m_Name;
		ComPtr<ID3DBlob> m_VsShader;
		ComPtr<ID3DBlob> m_PsShader;

		/**
		 * \brief Shader输入布局
		 */
		std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;
	};
}
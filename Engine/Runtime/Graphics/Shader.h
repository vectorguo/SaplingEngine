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

		/**
		 * \brief 获取顶点着色器指针
		 * \return 顶点着色器指针
		 */
		ID3DBlob* GetVsShader() const
		{
			return m_VsShader.Get();
		}

		/**
		 * \brief 获取片段着色器指针
		 * \return 片段着色器指针
		 */
		ID3DBlob* GetPsShader() const
		{
			return m_PsShader.Get();
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
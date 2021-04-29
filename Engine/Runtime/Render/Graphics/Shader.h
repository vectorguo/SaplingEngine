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
		 * \brief ��ȡShader����
		 * \return Shader����
		 */
		const std::string& GetName() const
		{
			return m_Name;
		}

		/**
		 * \brief ��ȡShader���ƶ�Ӧ��Hashֵ
		 * \return Shader���ƶ�Ӧ��Hashֵ
		 */
		size_t GetHashValue() const
		{
			return m_HashValue;
		}

		/**
		 * \brief ��ȡShader����
		 * \return Shader����
		 */
		uint8_t GetType() const
		{
			return m_Type;
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
		 * \brief ��ȡShader�����벼��
		 * \return Shader�����벼��
		 */
		const std::vector<D3D12_INPUT_ELEMENT_DESC>* GetInputLayout() const
		{
			return &m_InputLayout;
		}
		
	private:
		/**
		 * \brief Shader����
		 */
		std::string m_Name;

		/**
		 * \brief Shader���ƶ�Ӧ��HashValue
		 */
		size_t m_HashValue;

		/**
		 * \brief Shader���ͣ��������ֲ�ͬshader��Ӧ��SpecialCBuffer
		 */
		uint8_t m_Type;

		ComPtr<ID3DBlob> m_VsShader;
		ComPtr<ID3DBlob> m_PsShader;

		/**
		 * \brief Shader���벼��
		 */
		std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;
	};
}
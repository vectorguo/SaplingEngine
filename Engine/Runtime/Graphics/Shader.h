#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	using std::string;
	using std::wstring;
	
	class Shader
	{
	public:
		Shader(const string& name, const wstring& path);
		Shader(string&& name, const wstring& path);
		Shader(const string& name, const wstring& path, const string& vEntryPoint, const string& pEntryPoint);
		Shader(string&& name, const wstring& path, const string& vEntryPoint, const string& pEntryPoint);
		~Shader();
		
		Shader(const Shader&) = delete;
		Shader(Shader&&) = delete;
		Shader& operator=(const Shader&) = delete;
		Shader& operator=(Shader&&) = delete;

		/**
		 * \brief ��ȡShader����
		 * \return Shader����
		 */
		const string& GetName() const
		{
			return m_Name;
		}

		/**
		 * \brief ��ȡ������ɫ��ָ��
		 * \return ������ɫ��ָ��
		 */
		ID3DBlob* GetVsShader() const
		{
			return m_VsShader.Get();
		}

		/**
		 * \brief ��ȡƬ����ɫ��ָ��
		 * \return Ƭ����ɫ��ָ��
		 */
		ID3DBlob* GetPsShader() const
		{
			return m_PsShader.Get();
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
		string m_Name;
		ComPtr<ID3DBlob> m_VsShader;
		ComPtr<ID3DBlob> m_PsShader;

		/**
		 * \brief Shader���벼��
		 */
		std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;
	};
}
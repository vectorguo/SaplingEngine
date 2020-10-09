#pragma once

#include "Application/DirectX12/D3D12Header.h"

namespace SaplingEngine
{
	class Shader
	{
	public:
		Shader(const std::string& name, const std::wstring& path);
		Shader(std::string&& name, const std::wstring& path);
		Shader(const std::string& name, const std::wstring& path, const std::string& vEntryPoint, const std::string& pEntryPoint);
		Shader(std::string&& name, const std::wstring& path, const std::string& vEntryPoint, const std::string& pEntryPoint);
		~Shader();
		
		Shader(const Shader&) = delete;
		Shader(Shader&&) = delete;
		Shader& operator=(const Shader&) = delete;
		Shader& operator=(Shader&&) = delete;
		
	private:
		std::string m_Name;
		ComPtr<ID3DBlob> m_VsShader;
		ComPtr<ID3DBlob> m_PsShader;
	};
}
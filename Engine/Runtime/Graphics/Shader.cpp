#include "Shader.h"

namespace SaplingEngine
{
	/**
	 * \brief ±‡“ÎShader
	 * \param fileName 
	 * \param defines 
	 * \param entryPoint 
	 * \param target 
	 * \return 
	 */
	ComPtr<ID3DBlob> CompileShader(const std::wstring& fileName, const D3D_SHADER_MACRO* defines, const std::string& entryPoint, const std::string& target)
	{
		uint32_t compileFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
		compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ComPtr<ID3DBlob> byteCode = nullptr;
		ComPtr<ID3DBlob> errors = nullptr;
		const auto result = D3DCompileFromFile(fileName.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);
		if (errors != nullptr)
		{
			OutputDebugStringA(static_cast<char*>(errors->GetBufferPointer()));
		}
		ThrowIfFailed(result);

		return byteCode;
	}

	Shader::Shader(const std::string& name, const std::wstring& path) : Shader(name, path, "Vert", "Frag")
	{
		
	}

	Shader::Shader(std::string&& name, const std::wstring& path) : Shader(std::move(name), path, "Vert", "Frag")
	{
	}

	Shader::Shader(const std::string& name, const std::wstring& path, const std::string& vEntryPoint, const std::string& pEntryPoint) : m_Name(name)
	{
	}

	Shader::Shader(std::string&& name, const std::wstring& path, const std::string& vEntryPoint, const std::string& pEntryPoint) : m_Name(std::move(name))
	{
	}

	Shader::~Shader()
	{
	}
}

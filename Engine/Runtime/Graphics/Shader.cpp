#include "Shader.h"

namespace SaplingEngine
{
	/**
	 * \brief 编译Shader
	 * \param fileName Shader所在的文件路径
	 * \param defines 宏定义
	 * \param entryPoint 入口函数名称
	 * \param target 版本
	 * \return Shader
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
		m_VsShader = CompileShader(path, nullptr, vEntryPoint, "vs_5_1");
		m_PsShader = CompileShader(path, nullptr, pEntryPoint, "ps_5_1");
		
		m_InputLayout.reserve(5);
		m_InputLayout.push_back({ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayout.push_back({ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayout.push_back({ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayout.push_back({ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayout.push_back({ "TEXCOORD",	1, DXGI_FORMAT_R32G32_FLOAT,		0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	}

	Shader::Shader(std::string&& name, const std::wstring& path, const std::string& vEntryPoint, const std::string& pEntryPoint) : m_Name(std::move(name))
	{
		m_VsShader = CompileShader(path, nullptr, vEntryPoint, "vs_5_1");
		m_PsShader = CompileShader(path, nullptr, pEntryPoint, "ps_5_1");

		m_InputLayout.reserve(5);
		m_InputLayout.push_back({ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayout.push_back({ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayout.push_back({ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayout.push_back({ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayout.push_back({ "TEXCOORD",	1, DXGI_FORMAT_R32G32_FLOAT,		0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	}

	Shader::~Shader() = default;
}

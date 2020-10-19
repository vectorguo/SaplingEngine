#include "Shader.h"

namespace SaplingEngine
{
	const char* InputLayoutSemanticNames[4] = { "POSITION", "NORMAL", "COLOR", "TEXCOORD" };
	
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

	Shader::Shader(const XmlNode* pShaderNode) : m_Name(pShaderNode->first_attribute("name")->value())
	{
		const auto path = CharToWChar(pShaderNode->first_attribute("path")->value());
		m_VsShader = CompileShader(path, nullptr, pShaderNode->first_attribute("vert")->value(), "vs_5_1");
		m_PsShader = CompileShader(path, nullptr, pShaderNode->first_attribute("frag")->value(), "ps_5_1");

		m_InputLayout.reserve(std::stoi(pShaderNode->first_attribute("inputLayoutCount")->value()));
		for (auto* pChild = pShaderNode->first_node(); pChild; pChild = pChild->next_sibling())
		{
			const auto* name = pChild->first_attribute("name")->value();
			const auto* index = pChild->first_attribute("index")->value();
			const auto* format = pChild->first_attribute("format")->value();
			const auto* inputSlot = pChild->first_attribute("inputSlot")->value();
			const auto* offset = pChild->first_attribute("offset")->value();
			const auto* inputSlotClass = pChild->first_attribute("inputSlotClass")->value();
			const auto* stepRate = pChild->first_attribute("stepRate")->value();
			
			m_InputLayout.push_back({
				InputLayoutSemanticNames[std::stoi(name)],
				static_cast<uint32_t>(std::stoi(index)),
				static_cast<DXGI_FORMAT>(std::stoi(format)),
				static_cast<uint32_t>(std::stoi(inputSlot)),
				static_cast<uint32_t>(std::stoi(offset)),
				static_cast<D3D12_INPUT_CLASSIFICATION>(std::stoi(inputSlotClass)),
				static_cast<uint32_t>(std::stoi(stepRate))});
		}
	}

	Shader::~Shader() = default;
}

#include "Shader.h"
#include "ShaderManager.h"

namespace SaplingEngine
{
	/**
	 * \brief 初始化
	 */
	void ShaderManager::Initialize()
	{
		auto* pShader = new Shader("Color", L"Resources/Shaders/color.hlsl");
		m_Shaders.insert({ pShader->GetName(), pShader });
	}

	/**
	 * \brief 销毁
	 */
	void ShaderManager::Destroy()
	{
		for (auto iter = m_Shaders.begin(); iter != m_Shaders.end(); ++iter)
		{
			delete iter->second;
		}
	}

	/**
	 * \brief 获取Shader
	 * \param shaderName Shader名称
	 * \return Shader
	 */
	const Shader* ShaderManager::GetShader(const std::string& shaderName) const
	{
		const auto iter = m_Shaders.find(shaderName);
		return iter == m_Shaders.end() ? nullptr : iter->second;
	}
}

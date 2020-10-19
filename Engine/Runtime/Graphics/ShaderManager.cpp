#include "Shader.h"
#include "ShaderManager.h"

#include "Application/GameSetting.h"

namespace SaplingEngine
{
	/**
	 * \brief 初始化
	 */
	void ShaderManager::Initialize()
	{
		const auto* pShadersNode = GameSetting::Instance()->GetNode("shaders");
		for (auto* pChild = pShadersNode->first_node(); pChild; pChild = pChild->next_sibling())
		{
			auto* pShader = new Shader(pChild);
			m_Shaders.insert({ pShader->GetName(), pShader });
		}
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

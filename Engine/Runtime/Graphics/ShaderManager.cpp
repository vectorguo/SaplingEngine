#include "ShaderManager.h"

namespace SaplingEngine
{
	/**
	 * \brief ³õÊ¼»¯
	 */
	void ShaderManager::Initialize()
	{
		auto* pShader = new Shader("Color", L"../../../Resources/Shaders/color.hlsl");
		m_Shaders.insert({ pShader->GetName(), pShader });
	}

	/**
	 * \brief Ïú»Ù
	 */
	void ShaderManager::Destroy()
	{
		for (auto iter = m_Shaders.begin(); iter != m_Shaders.end(); ++iter)
		{
			delete iter->second;
		}
	}
}

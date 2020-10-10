#include "ShaderManager.h"

namespace SaplingEngine
{
	/**
	 * \brief ��ʼ��
	 */
	void ShaderManager::Initialize()
	{
		auto* pShader = new Shader("Color", L"../../../Resources/Shaders/color.hlsl");
		m_Shaders.insert({ pShader->GetName(), pShader });
	}

	/**
	 * \brief ����
	 */
	void ShaderManager::Destroy()
	{
		for (auto iter = m_Shaders.begin(); iter != m_Shaders.end(); ++iter)
		{
			delete iter->second;
		}
	}
}

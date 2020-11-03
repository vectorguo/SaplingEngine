#include "Shader.h"
#include "ShaderManager.h"

namespace SaplingEngine
{
	/**
	 * \brief ��ʼ��
	 */
	void ShaderManager::Initialize()
	{
		//����XML����
		auto* pDocumentFile = new XmlDocumentFile("Resources/Configs/ShaderConfig.xml");
		auto* pDocument = new XmlDocument();
		pDocument->parse<0>(pDocumentFile->data());

		const auto* pRootNode = pDocument->first_node();
		const auto* pShadersNode = pRootNode->first_node();
		for (auto* pChild = pShadersNode->first_node(); pChild; pChild = pChild->next_sibling())
		{
			auto* pShader = new Shader(pChild);
			m_Shaders.insert({ pShader->GetName(), pShader });
		}

		//ж��XML
		delete pDocument;
		delete pDocumentFile;
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

	/**
	 * \brief ��ȡShader
	 * \param shaderName Shader����
	 * \return Shader
	 */
	const Shader* ShaderManager::GetShader(const std::string& shaderName) const
	{
		const auto iter = m_Shaders.find(shaderName);
		return iter == m_Shaders.end() ? nullptr : iter->second;
	}
}

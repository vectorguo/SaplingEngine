#include "Shader.h"
#include "ShaderManager.h"

namespace SaplingEngine
{
	ShaderManager::ShaderMap ShaderManager::shaders;

	/**
	 * \brief 初始化
	 */
	void ShaderManager::Initialize()
	{
		//加载XML配置
		auto* pDocumentFile = new XmlDocumentFile("Resources/Configs/ShaderConfig.xml");
		auto* pDocument = new XmlDocument();
		pDocument->parse<0>(pDocumentFile->data());

		const auto* pRootNode = pDocument->first_node();
		const auto* pShadersNode = pRootNode->first_node();
		for (auto* pChild = pShadersNode->first_node(); pChild; pChild = pChild->next_sibling())
		{
			auto* pShader = new Shader(pChild);
			shaders.insert({ pShader->GetHashValue(), pShader });
		}

		//卸载XML
		delete pDocument;
		delete pDocumentFile;
	}

	/**
	 * \brief 销毁
	 */
	void ShaderManager::Destroy()
	{
		for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
		{
			delete iter->second;
		}
	}

	/**
	 * \brief	获取Shader
	 * \param	shaderName	Shader名称
	 * \return	Shader
	 */
	const Shader* ShaderManager::GetShader(const std::string& shaderName)
	{
		return GetShader(StringToHash(shaderName));
	}

	/**
	 * \brief	获取Shader
	 * \param	shaderID	ShaderID
	 * \return	Shader
	 */
	const Shader* ShaderManager::GetShader(size_t shaderID)
	{
		const auto iter = shaders.find(shaderID);
		return iter == shaders.end() ? nullptr : iter->second;
	}
}

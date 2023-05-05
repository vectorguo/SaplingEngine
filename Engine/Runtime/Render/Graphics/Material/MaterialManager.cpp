#include "Material.h"
#include "MaterialManager.h"

namespace SaplingEngine
{
	std::map<std::string, MaterialSptr> MaterialManager::sharedMaterials;
	
	/**
	 * \brief	初始化
	 */
	void MaterialManager::Initialize()
	{
		//加载XML配置
		auto* pDocumentFile = new XmlDocumentFile("Resources/Configs/MaterialConfig.xml");
		auto* pDocument = new XmlDocument();
		pDocument->parse<0>(pDocumentFile->data());

		const auto* pRootNode = pDocument->first_node();
		const auto* pMaterialsNode = pRootNode->first_node();
		for (auto* pChild = pMaterialsNode->first_node(); pChild; pChild = pChild->next_sibling())
		{
			auto materialName = XmlGetAttributeValue<const char*>(pChild, "name");
			auto sharedMaterial = MakeShared<Material>();
			sharedMaterial->Deserialize(pChild);
			sharedMaterials.emplace(materialName, sharedMaterial);
		}

		//卸载XML
		delete pDocument;
		delete pDocumentFile;
	}
	
	/**
	 * \brief	销毁
	 */
	void MaterialManager::Destroy()
	{
		sharedMaterials.clear();
	}
	
	/**
	 * \brief	创建材质
	 * \param	shaderID		材质所用的ShaderID
	 * \return	材质的智能指针
	 */
	MaterialSptr MaterialManager::CreateMaterial(size_t shaderID)
	{
		auto material = MakeShared<Material>();
		material->SetShader(shaderID);
		return material;
	}
	
	/**
	 * \brief	创建材质
	 * \param	shaderID		材质所用的ShaderID
	 * \return	材质的智能指针
	 */
	MaterialSptr MaterialManager::CreateMaterial(const std::string& shaderName)
	{
		auto material = MakeShared<Material>();
		material->SetShader(shaderName);
		return material;
	}

	/**
	 * \brief	获取共享材质
	 * \param	materialName	材质名称
	 * \return	共享材质的智能指针
	 */
	MaterialSptr MaterialManager::GetSharedMaterial(const std::string& materialName)
	{
		auto iter = sharedMaterials.find(materialName);
		if (iter == sharedMaterials.end())
		{
			throw Exception("获取不存在的共享材质");
		}
		else
		{
			return iter->second;
		}
	}

	/**
	 * \brief	获取或创建共享材质
	 * \param	materialName	材质名称
	 * \param	shaderID		材质所用的ShaderID
	 * \return	共享材质的智能指针
	 */
	MaterialSptr MaterialManager::GetOrCreateSharedMaterial(const std::string& materialName, size_t shaderID)
	{
		auto iter = sharedMaterials.find(materialName);
		if (iter == sharedMaterials.end())
		{
			auto sharedMaterial = MakeShared<Material>();
			sharedMaterial->SetShader(shaderID);
			sharedMaterials.emplace(materialName, sharedMaterial);
			return sharedMaterial;
		}
		else
		{
			return iter->second;
		}
	}

	/**
	 * \brief	获取或创建共享材质
	 * \param	materialName	材质名称
	 * \param	shaderName		材质所用的Shader名称
	 */
	MaterialSptr MaterialManager::GetOrCreateSharedMaterial(const std::string& materialName, const std::string& shaderName)
	{
		auto iter = sharedMaterials.find(materialName);
		if (iter == sharedMaterials.end())
		{
			auto sharedMaterial = MakeShared<Material>();
			sharedMaterial->SetShader(shaderName);
			sharedMaterials.emplace(materialName, sharedMaterial);
			return sharedMaterial;
		}
		else
		{
			return iter->second;
		}
	}
}
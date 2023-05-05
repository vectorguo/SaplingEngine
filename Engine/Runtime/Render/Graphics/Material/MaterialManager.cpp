#include "Material.h"
#include "MaterialManager.h"

namespace SaplingEngine
{
	std::map<std::string, MaterialSptr> MaterialManager::sharedMaterials;
	
	/**
	 * \brief	��ʼ��
	 */
	void MaterialManager::Initialize()
	{
		//����XML����
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

		//ж��XML
		delete pDocument;
		delete pDocumentFile;
	}
	
	/**
	 * \brief	����
	 */
	void MaterialManager::Destroy()
	{
		sharedMaterials.clear();
	}
	
	/**
	 * \brief	��������
	 * \param	shaderID		�������õ�ShaderID
	 * \return	���ʵ�����ָ��
	 */
	MaterialSptr MaterialManager::CreateMaterial(size_t shaderID)
	{
		auto material = MakeShared<Material>();
		material->SetShader(shaderID);
		return material;
	}
	
	/**
	 * \brief	��������
	 * \param	shaderID		�������õ�ShaderID
	 * \return	���ʵ�����ָ��
	 */
	MaterialSptr MaterialManager::CreateMaterial(const std::string& shaderName)
	{
		auto material = MakeShared<Material>();
		material->SetShader(shaderName);
		return material;
	}

	/**
	 * \brief	��ȡ�������
	 * \param	materialName	��������
	 * \return	������ʵ�����ָ��
	 */
	MaterialSptr MaterialManager::GetSharedMaterial(const std::string& materialName)
	{
		auto iter = sharedMaterials.find(materialName);
		if (iter == sharedMaterials.end())
		{
			throw Exception("��ȡ�����ڵĹ������");
		}
		else
		{
			return iter->second;
		}
	}

	/**
	 * \brief	��ȡ�򴴽��������
	 * \param	materialName	��������
	 * \param	shaderID		�������õ�ShaderID
	 * \return	������ʵ�����ָ��
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
	 * \brief	��ȡ�򴴽��������
	 * \param	materialName	��������
	 * \param	shaderName		�������õ�Shader����
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
#include "ResourceManager.h"
#include "ResourceLoader.h"

namespace SaplingEngine
{
	ResourceManager::ResourceMap							ResourceManager::m_Resources;
	std::map<EResourceType, ResourceManager::LoadDelegate>	ResourceManager::m_ResourceLoaders;
	std::map<std::string, ResourceManager::MeshConfig>		ResourceManager::m_MeshConfigs;

	/**
	 * \brief ��ʼ��
	 * \return ��ʼ���Ƿ�ɹ�
	 */
	bool ResourceManager::Initialize()
	{
		//������Դ������
		m_ResourceLoaders.emplace(EResourceType::Mesh, LoadMeshResource);

		//������Դ����
		LoadResourceConfigs();

		return true;
	}

	/**
	 * \brief ����Mesh����
	 */
	void ResourceManager::LoadResourceConfigs()
	{
		auto* pDocumentFile = new XmlDocumentFile("Resources/Configs/ResourceConfig.xml");
		auto* pDocument = new XmlDocument();
		pDocument->parse<0>(pDocumentFile->data());
		const auto* pRootNode = pDocument->first_node();

		//����Mesh��Դ����
		const auto* pMeshesNode = pRootNode->first_node();
		for (auto* pChild = pMeshesNode->first_node(); pChild; pChild = pChild->next_sibling())
		{
			const auto* meshName = XmlGetAttributeValue<const char*>(pChild, "name");
			const auto* meshPath = XmlGetAttributeValue<const char*>(pChild, "path");
			const auto vertexCount = XmlGetAttributeValue<uint32_t>(pChild, "vertexCount");
			const auto indexCount = XmlGetAttributeValue<uint32_t>(pChild, "indexCount");
			m_MeshConfigs.emplace(meshName, std::make_tuple(meshPath, vertexCount, indexCount));
		}

		//ж��XML
		delete pDocument;
		delete pDocumentFile;
	}
}

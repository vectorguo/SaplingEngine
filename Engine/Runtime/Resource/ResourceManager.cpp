#include "ResourceManager.h"
#include "ResourceLoader.h"

namespace SaplingEngine
{
	/**
	 * \brief ��ʼ��
	 * \return ��ʼ���Ƿ�ɹ�
	 */
	bool ResourceManager::Initialize()
	{
		//������Դ������
		m_ResourceLoaders.insert_or_assign(EResourceType::Mesh, LoadMeshResource);

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
			m_MeshConfigs.insert_or_assign(meshName, std::make_tuple(meshPath, vertexCount, indexCount));
		}

		//ж��XML
		delete pDocument;
		delete pDocumentFile;
	}
}

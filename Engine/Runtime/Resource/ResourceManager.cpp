#include "ResourceManager.h"
#include "ResourceLoader.h"

namespace SaplingEngine
{
	/**
	 * \brief 初始化
	 * \return 初始化是否成功
	 */
	bool ResourceManager::Initialize()
	{
		//设置资源加载器
		m_ResourceLoaders.insert_or_assign(EResourceType::Mesh, LoadMeshResource);

		//加载资源配置
		LoadResourceConfigs();

		return true;
	}

	/**
	 * \brief 加载Mesh配置
	 */
	void ResourceManager::LoadResourceConfigs()
	{
		auto* pDocumentFile = new XmlDocumentFile("Resources/Configs/ResourceConfig.xml");
		auto* pDocument = new XmlDocument();
		pDocument->parse<0>(pDocumentFile->data());
		const auto* pRootNode = pDocument->first_node();

		//加载Mesh资源配置
		const auto* pMeshesNode = pRootNode->first_node();
		for (auto* pChild = pMeshesNode->first_node(); pChild; pChild = pChild->next_sibling())
		{
			const auto* meshName = XmlGetAttributeValue<const char*>(pChild, "name");
			const auto* meshPath = XmlGetAttributeValue<const char*>(pChild, "path");
			const auto vertexCount = XmlGetAttributeValue<uint32_t>(pChild, "vertexCount");
			const auto indexCount = XmlGetAttributeValue<uint32_t>(pChild, "indexCount");
			m_MeshConfigs.insert_or_assign(meshName, std::make_tuple(meshPath, vertexCount, indexCount));
		}

		//卸载XML
		delete pDocument;
		delete pDocumentFile;
	}
}

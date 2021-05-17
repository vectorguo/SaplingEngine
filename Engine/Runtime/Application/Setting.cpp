#include "Setting.h"

namespace SaplingEngine
{
	//静态成员初始化
	uint32_t	Setting::screenWidth = 0;
	uint32_t	Setting::screenHeight = 0;
	std::string Setting::startSceneName;
	std::string Setting::startScenePath;
	
	/**
	 * \brief 初始化
	 * \return 是否初始化成功
	 */
	bool Setting::Initialize()
	{
		//加载XML配置
		auto* pDocumentFile = new XmlDocumentFile("Resources/Configs/ApplicationConfig.xml");
		auto* pDocument = new XmlDocument();
		pDocument->parse<0>(pDocumentFile->data());

		//读取root节点
		const auto* pRootNode = pDocument->first_node();

		//读取窗口宽度和高度
		screenWidth	= XmlGetNodeValue<uint32_t>(pRootNode, "windowWidth");
		screenHeight	= XmlGetNodeValue<uint32_t>(pRootNode, "windowHeight");

		//读取启动场景配置
		const auto* pSceneNode = pRootNode->first_node("startScene");
		startSceneName = XmlGetAttributeValue<const char*>(pSceneNode, "sceneName");
		startScenePath = XmlGetAttributeValue<const char*>(pSceneNode, "scenePath");

		//卸载XML
		delete pDocument;
		delete pDocumentFile;
		
		return true;
	}
	
	/**
	 * \brief	初始化
	 * \param	width		屏幕宽度
	 * \param	height		屏幕高度
	 * \return	是否初始化成功
	 */
	bool Setting::Initialize(uint32_t width, uint32_t height)
	{
		//读取窗口宽度和高度
		screenWidth = width;
		screenHeight = height;

		//加载XML配置
		auto* pDocumentFile = new XmlDocumentFile("Resources/Configs/ApplicationConfig.xml");
		auto* pDocument = new XmlDocument();
		pDocument->parse<0>(pDocumentFile->data());

		//读取root节点
		const auto* pRootNode = pDocument->first_node();

		//读取启动场景配置
		const auto* pSceneNode = pRootNode->first_node("startScene");
		startSceneName = XmlGetAttributeValue<const char*>(pSceneNode, "sceneName");
		startScenePath = XmlGetAttributeValue<const char*>(pSceneNode, "scenePath");

		//卸载XML
		delete pDocument;
		delete pDocumentFile;

		return true;
	}
}

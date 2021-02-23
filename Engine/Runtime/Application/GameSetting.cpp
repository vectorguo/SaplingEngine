#include "GameSetting.h"

namespace SaplingEngine
{
	//静态成员初始化
	uint32_t	GameSetting::m_ScreenWidth = 0;
	uint32_t	GameSetting::m_ScreenHeight = 0;
	std::string GameSetting::m_StartSceneName;
	std::string GameSetting::m_StartScenePath;
	
	/**
	 * \brief 初始化
	 * \return 是否初始化成功
	 */
	bool GameSetting::Initialize()
	{
		//加载XML配置
		auto* pDocumentFile = new XmlDocumentFile("Resources/Configs/ApplicationConfig.xml");
		auto* pDocument = new XmlDocument();
		pDocument->parse<0>(pDocumentFile->data());

		//读取root节点
		const auto* pRootNode = pDocument->first_node();

		//读取窗口宽度和高度
		m_ScreenWidth	= XmlGetNodeValue<uint32_t>(pRootNode, "windowWidth");
		m_ScreenHeight	= XmlGetNodeValue<uint32_t>(pRootNode, "windowHeight");

		//读取启动场景配置
		const auto* pSceneNode = pRootNode->first_node("startScene");
		m_StartSceneName = XmlGetAttributeValue<const char*>(pSceneNode, "sceneName");
		m_StartScenePath = XmlGetAttributeValue<const char*>(pSceneNode, "scenePath");

		//卸载XML
		delete pDocument;
		delete pDocumentFile;
		
		return true;
	}
}

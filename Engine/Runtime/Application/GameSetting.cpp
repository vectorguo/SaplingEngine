#include "GameSetting.h"

namespace SaplingEngine
{
	GameSetting::~GameSetting()
	{
		delete m_pDocumentFile;
		delete m_pDocument;
	}

	/**
	 * \brief 初始化
	 * \return 是否初始化成功
	 */
	bool GameSetting::Initialize()
	{
		//加载XML配置
		m_pDocumentFile = new XmlDocumentFile("Resources/Configs/ApplicationConfig.xml");
		m_pDocument = new XmlDocument();
		m_pDocument->parse<0>(m_pDocumentFile->data());

		//读取root节点
		m_pRootNode = m_pDocument->first_node();

		//读取窗口宽度和高度
		m_ScreenWidth	= XmlGetNodeValue<uint32_t>(m_pRootNode, "windowWidth");
		m_ScreenHeight	= XmlGetNodeValue<uint32_t>(m_pRootNode, "windowHeight");

		//读取启动场景配置
		const auto* pSceneNode = m_pRootNode->first_node("startScene");
		m_StartSceneName = XmlGetAttributeValue<char*>(pSceneNode, "sceneName");
		m_StartScenePath = XmlGetAttributeValue<char*>(pSceneNode, "scenePath");

		return true;
	}
}

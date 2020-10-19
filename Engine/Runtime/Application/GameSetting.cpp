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
		const auto* pWindowWidthNode = m_pRootNode->first_node("windowWidth");
		const auto* pWindowHeightNode = m_pRootNode->first_node("windowHeight");
		m_ScreenWidth = std::stoi(pWindowWidthNode->value());
		m_ScreenHeight = std::stoi(pWindowHeightNode->value());

		//读取启动场景配置
		const auto* pSceneNode = m_pRootNode->first_node("startScene");
		m_StartSceneName = pSceneNode->first_attribute("sceneName")->value();
		m_StartScenePath = pSceneNode->first_attribute("scenePath")->value();

		return true;
	}
}

#include "GameSetting.h"

namespace SaplingEngine
{
	/**
	 * \brief 初始化
	 * \return 是否初始化成功
	 */
	bool GameSetting::Initialize()
	{
		//加载XML配置
		XmlDocumentFile documentFile("Resources/Configs/ApplicationConfig.xml");
		m_SettingDocument.parse<0>(documentFile.data());

		//读取root节点
		auto* pRoot = m_SettingDocument.first_node();

		//读取窗口宽度和高度
		auto* pWindowWidthNode = pRoot->first_node("windowWidth");
		auto* pWindowHeightNode = pRoot->first_node("windowHeight");
		m_ScreenWidth = std::stoi(pWindowWidthNode->value());
		m_ScreenHeight = std::stoi(pWindowHeightNode->value());

		//读取启动场景配置
		auto* pSceneNode = pRoot->first_node("startScene");
		m_StartScenePath = pSceneNode->value();

		return true;
	}
}

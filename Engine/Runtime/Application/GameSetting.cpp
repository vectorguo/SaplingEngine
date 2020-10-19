#include "GameSetting.h"

namespace SaplingEngine
{
	GameSetting::~GameSetting()
	{
		delete m_pDocumentFile;
		delete m_pDocument;
	}

	/**
	 * \brief ��ʼ��
	 * \return �Ƿ��ʼ���ɹ�
	 */
	bool GameSetting::Initialize()
	{
		//����XML����
		m_pDocumentFile = new XmlDocumentFile("Resources/Configs/ApplicationConfig.xml");
		m_pDocument = new XmlDocument();
		m_pDocument->parse<0>(m_pDocumentFile->data());

		//��ȡroot�ڵ�
		m_pRootNode = m_pDocument->first_node();

		//��ȡ���ڿ�Ⱥ͸߶�
		const auto* pWindowWidthNode = m_pRootNode->first_node("windowWidth");
		const auto* pWindowHeightNode = m_pRootNode->first_node("windowHeight");
		m_ScreenWidth = std::stoi(pWindowWidthNode->value());
		m_ScreenHeight = std::stoi(pWindowHeightNode->value());

		//��ȡ������������
		const auto* pSceneNode = m_pRootNode->first_node("startScene");
		m_StartSceneName = pSceneNode->first_attribute("sceneName")->value();
		m_StartScenePath = pSceneNode->first_attribute("scenePath")->value();

		return true;
	}
}

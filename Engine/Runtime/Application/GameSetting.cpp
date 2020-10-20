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
		m_ScreenWidth	= XmlGetNodeValue<uint32_t>(m_pRootNode, "windowWidth");
		m_ScreenHeight	= XmlGetNodeValue<uint32_t>(m_pRootNode, "windowHeight");

		//��ȡ������������
		const auto* pSceneNode = m_pRootNode->first_node("startScene");
		m_StartSceneName = XmlGetAttributeValue<char*>(pSceneNode, "sceneName");
		m_StartScenePath = XmlGetAttributeValue<char*>(pSceneNode, "scenePath");

		return true;
	}
}

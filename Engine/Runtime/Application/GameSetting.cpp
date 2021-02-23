#include "GameSetting.h"

namespace SaplingEngine
{
	//��̬��Ա��ʼ��
	uint32_t	GameSetting::m_ScreenWidth = 0;
	uint32_t	GameSetting::m_ScreenHeight = 0;
	std::string GameSetting::m_StartSceneName;
	std::string GameSetting::m_StartScenePath;
	
	/**
	 * \brief ��ʼ��
	 * \return �Ƿ��ʼ���ɹ�
	 */
	bool GameSetting::Initialize()
	{
		//����XML����
		auto* pDocumentFile = new XmlDocumentFile("Resources/Configs/ApplicationConfig.xml");
		auto* pDocument = new XmlDocument();
		pDocument->parse<0>(pDocumentFile->data());

		//��ȡroot�ڵ�
		const auto* pRootNode = pDocument->first_node();

		//��ȡ���ڿ�Ⱥ͸߶�
		m_ScreenWidth	= XmlGetNodeValue<uint32_t>(pRootNode, "windowWidth");
		m_ScreenHeight	= XmlGetNodeValue<uint32_t>(pRootNode, "windowHeight");

		//��ȡ������������
		const auto* pSceneNode = pRootNode->first_node("startScene");
		m_StartSceneName = XmlGetAttributeValue<const char*>(pSceneNode, "sceneName");
		m_StartScenePath = XmlGetAttributeValue<const char*>(pSceneNode, "scenePath");

		//ж��XML
		delete pDocument;
		delete pDocumentFile;
		
		return true;
	}
}

#include "GameSetting.h"

namespace SaplingEngine
{
	//��̬��Ա��ʼ��
	uint32_t	GameSetting::screenWidth = 0;
	uint32_t	GameSetting::screenHeight = 0;
	std::string GameSetting::startSceneName;
	std::string GameSetting::startScenePath;
	
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
		screenWidth	= XmlGetNodeValue<uint32_t>(pRootNode, "windowWidth");
		screenHeight	= XmlGetNodeValue<uint32_t>(pRootNode, "windowHeight");

		//��ȡ������������
		const auto* pSceneNode = pRootNode->first_node("startScene");
		startSceneName = XmlGetAttributeValue<const char*>(pSceneNode, "sceneName");
		startScenePath = XmlGetAttributeValue<const char*>(pSceneNode, "scenePath");

		//ж��XML
		delete pDocument;
		delete pDocumentFile;
		
		return true;
	}
}

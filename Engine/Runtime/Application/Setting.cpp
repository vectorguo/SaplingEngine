#include "Setting.h"

namespace SaplingEngine
{
	//��̬��Ա��ʼ��
	bool		Setting::isEditorMode = false;
	uint32_t	Setting::screenWidth = 0;
	uint32_t	Setting::screenHeight = 0;
	std::string Setting::startSceneName;
	std::string Setting::startScenePath;
	
	/**
	 * \brief ��ʼ��
	 * \return �Ƿ��ʼ���ɹ�
	 */
	bool Setting::Initialize()
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

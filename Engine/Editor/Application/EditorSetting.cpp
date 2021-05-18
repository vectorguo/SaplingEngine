#include "Application/Setting.h"
#include "Editor/Application/EditorSetting.h"

using namespace SaplingEngine;

namespace SaplingEditor
{
	std::string EditorSetting::startSceneName;
	std::string EditorSetting::startScenePath;

	/**
	 * \brief	��ʼ��
	 * \param	width		��Ļ���
	 * \param	height		��Ļ�߶�
	 * \return	�Ƿ��ʼ���ɹ�
	 */
	void EditorSetting::Initialize(uint32_t width, uint32_t height)
	{
		Setting::Initialize();

		//���ñ༭��ģʽ
		Setting::isEditorMode = true;

		//�������ô��ڿ�Ⱥ͸߶�
		Setting::screenWidth = width;
		Setting::screenHeight = height;

		//����XML����
		auto* pDocumentFile = new XmlDocumentFile("Resources/Configs/EditorConfig.xml");
		auto* pDocument = new XmlDocument();
		pDocument->parse<0>(pDocumentFile->data());

		//��ȡroot�ڵ�
		const auto* pRootNode = pDocument->first_node();

		//��ȡ������������
		const auto* pSceneNode = pRootNode->first_node("startScene");
		startSceneName = SaplingEngine::XmlGetAttributeValue<const char*>(pSceneNode, "sceneName");
		startScenePath = SaplingEngine::XmlGetAttributeValue<const char*>(pSceneNode, "scenePath");

		//ж��XML
		delete pDocument;
		delete pDocumentFile;
	}
}
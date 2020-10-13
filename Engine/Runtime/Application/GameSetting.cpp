#include "GameSetting.h"

namespace SaplingEngine
{
	/**
	 * \brief ��ʼ��
	 * \return �Ƿ��ʼ���ɹ�
	 */
	bool GameSetting::Initialize()
	{
		//����XML����
		XmlDocumentFile documentFile("Resources/Configs/ApplicationConfig.xml");
		m_SettingDocument.parse<0>(documentFile.data());

		//��ȡroot�ڵ�
		auto* pRoot = m_SettingDocument.first_node();

		//��ȡ���ڿ�Ⱥ͸߶�
		auto* pWindowWidthNode = pRoot->first_node("windowWidth");
		auto* pWindowHeightNode = pRoot->first_node("windowHeight");
		m_ScreenWidth = std::stoi(pWindowWidthNode->value());
		m_ScreenHeight = std::stoi(pWindowHeightNode->value());

		//��ȡ������������
		auto* pSceneNode = pRoot->first_node("startScene");
		m_StartScenePath = pSceneNode->value();

		return true;
	}
}

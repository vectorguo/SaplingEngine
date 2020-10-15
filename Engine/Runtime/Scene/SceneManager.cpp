#include "SceneManager.h"

#include "Application/GameSetting.h"

namespace SaplingEngine
{
	/**
	 * \brief ��ʼ��
	 * \return �Ƿ��ʼ���ɹ�
	 */
	bool SceneManager::Initialize()
	{
		//������������
		const auto& startSceneName = GameSetting::Instance()->StartSceneName();
		const auto& startScenePath = GameSetting::Instance()->StartScenePath();
		LoadScene(startSceneName, startScenePath, false);
		
		return true;
	}

	/**
	 * \brief ���س���
	 * \param sceneName ��������
	 * \param scenePath ����·��
	 * \param additive �Ƿ��ѵ���ģʽ����
	 */
	void SceneManager::LoadScene(const std::string& sceneName, const std::string& scenePath, bool additive)
	{
		//����XML����
		XmlDocumentFile documentFile(scenePath.data());
		XmlDocument document;
		document.parse<0>(documentFile.data());

		
	}

	/**
	 * \brief ж�س���
	 * \param sceneName ��������
	 */
	void SceneManager::UnloadScene(const std::string& sceneName)
	{
		
	}
}

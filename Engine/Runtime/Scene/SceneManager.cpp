#include "SceneManager.h"

#include "Application/GameSetting.h"

namespace SaplingEngine
{
	/**
	 * \brief ��ʼ��
	 */
	void SceneManager::Initialize()
	{
		//������������
		const auto& startSceneName = GameSetting::Instance()->StartSceneName();
		const auto& startScenePath = GameSetting::Instance()->StartScenePath();
		LoadScene(startSceneName, startScenePath, false);
	}

	/**
	 * \brief ���س���
	 * \param sceneName ��������
	 * \param scenePath ����·��
	 * \param additive �Ƿ��ѵ���ģʽ����
	 */
	void SceneManager::LoadScene(const std::string& sceneName, const std::string& scenePath, bool additive)
	{
	}

	/**
	 * \brief ж�س���
	 * \param sceneName ��������
	 */
	void SceneManager::UnloadScene(const std::string& sceneName)
	{
	}
}

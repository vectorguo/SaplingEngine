#include "Scene.h"
#include "SceneManager.h"

#include "Application/GameSetting.h"

namespace SaplingEngine
{
	SceneManager::~SceneManager()
	{
		//���ٳ���
		for (auto iter = m_Scenes.begin(); iter != m_Scenes.end(); ++iter)
		{
			delete iter->second;
		}
	}

	/**
	 * \brief ��ʼ��
	 */
	void SceneManager::Initialize()
	{
		//������������
		const auto& startSceneName = GameSetting::Instance()->StartSceneName();
		const auto& startScenePath = GameSetting::Instance()->StartScenePath();
		LoadScene(startSceneName, startScenePath);
		SetActiveScene(startSceneName);
	}

	/**
	 * \brief ����
	 */
	void SceneManager::Update()
	{
		if (m_pActiveScene)
		{
			m_pActiveScene->Update();
		}
	}

	/**
	 * \brief ����
	 */
	void SceneManager::Destroy()
	{
		for (auto iter = m_Scenes.begin(); iter != m_Scenes.end(); ++iter)
		{
			iter->second->Destroy();
		}
	}

	/**
	 * \brief ���س���
	 * \param sceneName ��������
	 * \param scenePath ����·��
	 */
	void SceneManager::LoadScene(const std::string& sceneName, const std::string& scenePath)
	{
		//��ȡXML����
		XmlDocumentFile documentFile(scenePath.data());
		XmlDocument document;
		document.parse<0>(documentFile.data());

		//��������ʼ������
		auto* pScene = new Scene(sceneName);
		pScene->Initialize(document.first_node());
		m_Scenes.insert_or_assign(sceneName, pScene);
	}

	/**
	 * \brief ж�س���
	 * \param sceneName ��������
	 */
	void SceneManager::UnloadScene(const std::string& sceneName)
	{
		
	}

	/**
	 * \brief ���û����
	 * \param sceneName ��������
	 */
	void SceneManager::SetActiveScene(const std::string& sceneName)
	{
		const auto iter = m_Scenes.find(sceneName);
		if (iter == m_Scenes.end() || m_pActiveScene == iter->second)
		{
			return;
		}

		if (m_pActiveScene)
		{
			m_pActiveScene->SetActive(false);
		}
		m_pActiveScene = iter->second;
		m_pActiveScene->SetActive(true);
	}
}

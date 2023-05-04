#include "Scene.h"
#include "SceneManager.h"

#include "Runtime/Application/Setting.h"

namespace SaplingEngine
{
	std::map<std::string, Scene*>	SceneManager::m_Scenes;
	Scene*							SceneManager::m_pActiveScene = nullptr;

	/**
	 * \brief	��ʼ��
	 */
	void SceneManager::Initialize()
	{
		if (Setting::IsEditorMode())
		{
			return;
		}

		//������������
		const auto& startSceneName = Setting::StartSceneName();
		const auto& startScenePath = Setting::StartScenePath();
		LoadScene(startSceneName, startScenePath);
		SetActiveScene(startSceneName);
	}

	/**
	 * \brief	����
	 */
	void SceneManager::Update()
	{
		if (m_pActiveScene)
		{
			m_pActiveScene->Update();
		}
	}

	/**
	 * \brief	����
	 */
	void SceneManager::Destroy()
	{
		for (auto iter = m_Scenes.begin(); iter != m_Scenes.end(); ++iter)
		{
			iter->second->Destroy();
			delete iter->second;
		}
		m_Scenes.clear();
	}

	/**
	 * \brief	���س���
	 * \param	sceneName	��������
	 * \param	scenePath	����·��
	 */
	void SceneManager::LoadScene(const std::string& sceneName, const std::string& scenePath)
	{
		//��ȡXML����
		auto* pDocumentFile = new XmlDocumentFile(scenePath.data());
		auto* pDocument = new XmlDocument();
		pDocument->parse<0>(pDocumentFile->data());

		//��������ʼ������
		auto* pScene = new Scene(sceneName);
		m_Scenes.emplace(sceneName, pScene);
		SetActiveScene(sceneName);
		pScene->Initialize(pDocument->first_node());

		//ж��XML
		delete pDocument;
		delete pDocumentFile;
	}

	/**
	 * \brief	ж�س���
	 * \param	sceneName	��������
	 */
	void SceneManager::UnloadScene(const std::string& sceneName)
	{
		
	}

	/**
	 * \brief	���û����
	 * \param	sceneName	��������
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

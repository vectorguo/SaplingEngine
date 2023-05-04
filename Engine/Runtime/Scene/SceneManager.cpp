#include "Scene.h"
#include "SceneManager.h"

#include "Runtime/Application/Setting.h"

namespace SaplingEngine
{
	std::map<std::string, Scene*>	SceneManager::m_Scenes;
	Scene*							SceneManager::m_pActiveScene = nullptr;

	/**
	 * \brief	初始化
	 */
	void SceneManager::Initialize()
	{
		if (Setting::IsEditorMode())
		{
			return;
		}

		//加载启动场景
		const auto& startSceneName = Setting::StartSceneName();
		const auto& startScenePath = Setting::StartScenePath();
		LoadScene(startSceneName, startScenePath);
		SetActiveScene(startSceneName);
	}

	/**
	 * \brief	更新
	 */
	void SceneManager::Update()
	{
		if (m_pActiveScene)
		{
			m_pActiveScene->Update();
		}
	}

	/**
	 * \brief	销毁
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
	 * \brief	加载场景
	 * \param	sceneName	场景名称
	 * \param	scenePath	场景路径
	 */
	void SceneManager::LoadScene(const std::string& sceneName, const std::string& scenePath)
	{
		//读取XML配置
		auto* pDocumentFile = new XmlDocumentFile(scenePath.data());
		auto* pDocument = new XmlDocument();
		pDocument->parse<0>(pDocumentFile->data());

		//创建并初始化场景
		auto* pScene = new Scene(sceneName);
		m_Scenes.emplace(sceneName, pScene);
		SetActiveScene(sceneName);
		pScene->Initialize(pDocument->first_node());

		//卸载XML
		delete pDocument;
		delete pDocumentFile;
	}

	/**
	 * \brief	卸载场景
	 * \param	sceneName	场景名称
	 */
	void SceneManager::UnloadScene(const std::string& sceneName)
	{
		
	}

	/**
	 * \brief	设置活动场景
	 * \param	sceneName	场景名称
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

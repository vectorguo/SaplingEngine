#include "Scene.h"
#include "SceneManager.h"

#include "Application/GameSetting.h"

namespace SaplingEngine
{
	SceneManager::~SceneManager()
	{
		//销毁场景
		for (auto iter = m_Scenes.begin(); iter != m_Scenes.end(); ++iter)
		{
			delete iter->second;
		}
	}

	/**
	 * \brief 初始化
	 */
	void SceneManager::Initialize()
	{
		//加载启动场景
		const auto& startSceneName = GameSetting::Instance()->StartSceneName();
		const auto& startScenePath = GameSetting::Instance()->StartScenePath();
		LoadScene(startSceneName, startScenePath);
		SetActiveScene(startSceneName);
	}

	/**
	 * \brief 更新
	 */
	void SceneManager::Update()
	{
		if (m_pActiveScene)
		{
			m_pActiveScene->Update();
		}
	}

	/**
	 * \brief 销毁
	 */
	void SceneManager::Destroy()
	{
		for (auto iter = m_Scenes.begin(); iter != m_Scenes.end(); ++iter)
		{
			iter->second->Destroy();
		}
	}

	/**
	 * \brief 加载场景
	 * \param sceneName 场景名称
	 * \param scenePath 场景路径
	 */
	void SceneManager::LoadScene(const std::string& sceneName, const std::string& scenePath)
	{
		//读取XML配置
		XmlDocumentFile documentFile(scenePath.data());
		XmlDocument document;
		document.parse<0>(documentFile.data());

		//创建并初始化场景
		auto* pScene = new Scene(sceneName);
		pScene->Initialize(document.first_node());
		m_Scenes.insert_or_assign(sceneName, pScene);
	}

	/**
	 * \brief 卸载场景
	 * \param sceneName 场景名称
	 */
	void SceneManager::UnloadScene(const std::string& sceneName)
	{
		
	}

	/**
	 * \brief 设置活动场景
	 * \param sceneName 场景名称
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

#include "SceneManager.h"

#include "Application/GameSetting.h"

namespace SaplingEngine
{
	/**
	 * \brief 初始化
	 * \return 是否初始化成功
	 */
	bool SceneManager::Initialize()
	{
		//加载启动场景
		const auto& startSceneName = GameSetting::Instance()->StartSceneName();
		const auto& startScenePath = GameSetting::Instance()->StartScenePath();
		LoadScene(startSceneName, startScenePath, false);
		
		return true;
	}

	/**
	 * \brief 加载场景
	 * \param sceneName 场景名称
	 * \param scenePath 场景路径
	 * \param additive 是否已叠加模式加载
	 */
	void SceneManager::LoadScene(const std::string& sceneName, const std::string& scenePath, bool additive)
	{
		//加载XML配置
		XmlDocumentFile documentFile(scenePath.data());
		XmlDocument document;
		document.parse<0>(documentFile.data());

		
	}

	/**
	 * \brief 卸载场景
	 * \param sceneName 场景名称
	 */
	void SceneManager::UnloadScene(const std::string& sceneName)
	{
		
	}
}

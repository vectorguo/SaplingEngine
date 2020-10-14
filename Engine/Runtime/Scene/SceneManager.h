#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Scene;
	
	class SceneManager
	{
	public:
		SceneManager() = default;
		~SceneManager() = default;

		SINGLETON(SceneManager)

		/**
		 * \brief 初始化
		 */
		void Initialize();

		/**
		 * \brief 加载场景
		 * \param sceneName 场景名称
		 * \param scenePath 场景路径
		 * \param additive 是否已叠加模式加载
		 */
		void LoadScene(const std::string& sceneName, const std::string& scenePath, bool additive);

		/**
		 * \brief 卸载场景
		 * \param sceneName 场景名称
		 */
		void UnloadScene(const std::string& sceneName);
		
	private:
		/**
		 * \brief 所有加载完成的场景
		 */
		std::map<std::string, Scene*> m_Scenes;

		/**
		 * \brief 当前的活动场景
		 */
		Scene* m_pActiveScene = nullptr;
	};
}

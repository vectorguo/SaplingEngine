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
		 * \brief 更新
		 */
		void Update();

		/**
		 * \brief 销毁
		 */
		void Destroy();

		/**
		 * \brief 加载场景
		 * \param sceneName 场景名称
		 * \param scenePath 场景路径
		 */
		void LoadScene(const std::string& sceneName, const std::string& scenePath);

		/**
		 * \brief 卸载场景
		 * \param sceneName 场景名称
		 */
		void UnloadScene(const std::string& sceneName);

		/**
		 * \brief 获取活动场景
		 * \return 活动场景指针
		 */
		Scene* GetActiveScene() const
		{
			return m_pActiveScene;
		}
		
		/**
		 * \brief 设置活动场景
		 * \param sceneName 场景名称
		 */
		void SetActiveScene(const std::string& sceneName);

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

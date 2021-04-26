#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Scene;
	
	class SceneManager
	{
	public:
		/**
		 * \brief 初始化
		 */
		static void Initialize();

		/**
		 * \brief 更新
		 */
		static void Update();

		/**
		 * \brief 销毁
		 */
		static void Destroy();

		/**
		 * \brief 加载场景
		 * \param sceneName 场景名称
		 * \param scenePath 场景路径
		 */
		static void LoadScene(const std::string& sceneName, const std::string& scenePath);

		/**
		 * \brief 卸载场景
		 * \param sceneName 场景名称
		 */
		static void UnloadScene(const std::string& sceneName);

		/**
		 * \brief 获取活动场景
		 * \return 活动场景指针
		 */
		static Scene* GetActiveScene()
		{
			return m_pActiveScene;
		}
		
		/**
		 * \brief 设置活动场景
		 * \param sceneName 场景名称
		 */
		static void SetActiveScene(const std::string& sceneName);

	private:
		/**
		 * \brief 所有加载完成的场景
		 */
		static std::map<std::string, Scene*> m_Scenes;

		/**
		 * \brief 当前的活动场景
		 */
		static Scene* m_pActiveScene;
	};
}

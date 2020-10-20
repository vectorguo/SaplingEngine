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
		 * \brief ��ʼ��
		 */
		void Initialize();

		/**
		 * \brief ����
		 */
		void Update();

		/**
		 * \brief ����
		 */
		void Destroy();

		/**
		 * \brief ���س���
		 * \param sceneName ��������
		 * \param scenePath ����·��
		 */
		void LoadScene(const std::string& sceneName, const std::string& scenePath);

		/**
		 * \brief ж�س���
		 * \param sceneName ��������
		 */
		void UnloadScene(const std::string& sceneName);

		/**
		 * \brief ��ȡ�����
		 * \return �����ָ��
		 */
		Scene* GetActiveScene() const
		{
			return m_pActiveScene;
		}
		
		/**
		 * \brief ���û����
		 * \param sceneName ��������
		 */
		void SetActiveScene(const std::string& sceneName);

	private:
		/**
		 * \brief ���м�����ɵĳ���
		 */
		std::map<std::string, Scene*> m_Scenes;

		/**
		 * \brief ��ǰ�Ļ����
		 */
		Scene* m_pActiveScene = nullptr;
	};
}

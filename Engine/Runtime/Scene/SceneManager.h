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
		 * \brief ���س���
		 * \param sceneName ��������
		 * \param scenePath ����·��
		 * \param additive �Ƿ��ѵ���ģʽ����
		 */
		void LoadScene(const std::string& sceneName, const std::string& scenePath, bool additive);

		/**
		 * \brief ж�س���
		 * \param sceneName ��������
		 */
		void UnloadScene(const std::string& sceneName);
		
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

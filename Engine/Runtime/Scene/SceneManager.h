#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Scene;
	
	class SceneManager
	{
	public:
		/**
		 * \brief ��ʼ��
		 */
		static void Initialize();

		/**
		 * \brief ����
		 */
		static void Update();

		/**
		 * \brief ����
		 */
		static void Destroy();

		/**
		 * \brief ���س���
		 * \param sceneName ��������
		 * \param scenePath ����·��
		 */
		static void LoadScene(const std::string& sceneName, const std::string& scenePath);

		/**
		 * \brief ж�س���
		 * \param sceneName ��������
		 */
		static void UnloadScene(const std::string& sceneName);

		/**
		 * \brief ��ȡ�����
		 * \return �����ָ��
		 */
		static Scene* GetActiveScene()
		{
			return m_pActiveScene;
		}
		
		/**
		 * \brief ���û����
		 * \param sceneName ��������
		 */
		static void SetActiveScene(const std::string& sceneName);

	private:
		/**
		 * \brief ���м�����ɵĳ���
		 */
		static std::map<std::string, Scene*> m_Scenes;

		/**
		 * \brief ��ǰ�Ļ����
		 */
		static Scene* m_pActiveScene;
	};
}

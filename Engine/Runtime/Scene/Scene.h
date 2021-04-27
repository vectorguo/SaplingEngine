#pragma once

#include "GameObject/GameObject.h"

namespace SaplingEngine
{
	class Renderer;
	
	class Scene
	{
		friend class GameObject;
		
	public:
		explicit Scene(const std::string& sceneName);
		~Scene();

		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) = delete;
		
		/**
		 * \brief ��ʼ������
		 * \param pNode ���ýڵ�ָ��
		 */
		void Initialize(const XmlNode* pNode);

		/**
		 * \brief ����
		 */
		void Update();

		/**
		 * \brief ���ٳ���
		 */
		void Destroy();
		
		/**
		 * \brief �Ƿ��ǻ״̬
		 * \return �״̬
		 */
		bool IsActive() const
		{
			return m_IsActive;
		}
		
		/**
		 * \brief ���û״̬
		 * \param active �Ƿ�ʱ�״̬
		 */
		void SetActive(bool active);

		/**
		 * \brief ����GameObject
		 * \return GameObject
		 */
		GameObjectSptr CreateGameObject();
		
		/**
		 * \brief ����GameObject
		 * \param name GameObject����
		 * \return GameObject
		 */
		GameObjectSptr CreateGameObject(const std::string& name);

		/**
		 * \brief ����GameObject
		 * \param name GameObject����
		 * \return GameObject
		 */
		GameObjectSptr CreateGameObject(std::string&& name);

		/**
		 * \brief ��ȡ����
		 * \param name ��������
		 * \return ����ָ��
		 */
		GameObject* GetGameObject(const std::string& name);
		
		/**
		 * \brief ��ȡ����
		 * \param name ��������
		 * \return ��������ָ��
		 */
		GameObjectSptr GetGameObjectSptr(const std::string& name);

		/**
		 * \brief ��ȡ������Ϸ����
		 * \return ��Ϸ����
		 */
		const std::vector<GameObjectSptr>& GetGameObjects() const
		{
			return m_GameObjects;
		}

		/**
		 * \brief ��ȡ������Ϸ����
		 * \return ��Ϸ����
		 */
		std::vector<GameObjectSptr>& GetGameObjects()
		{
			return m_GameObjects;
		}

	private:
		/**
		 * \brief ����GameObject
		 * \return GameObject
		 */
		GameObjectSptr CreateGameObjectInternal();
		
	private:
		/**
		 * \brief ��������
		 */
		std::string m_SceneName;

		/**
		 * \brief �Ƿ�״̬
		 */
		bool m_IsActive;

		/**
		 * \brief �˳����е�GameObject
		 */
		std::vector<GameObjectSptr> m_GameObjects;
	};
}
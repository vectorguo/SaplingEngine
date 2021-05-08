#pragma once

#include "GameObject/GameObject.h"

namespace SaplingEngine
{
	class Renderer;
	
	class Scene
	{
		friend class GameObject;
		friend GameObjectSptr CreateGameObject();
		friend GameObjectSptr CreateGameObject(const std::string&);
		friend GameObjectSptr CreateGameObject(std::string&&);
		friend void DestroyGameObject(GameObject*);
		friend void DestroyGameObject(const GameObjectSptr&);
		
	public:
		explicit Scene(const std::string& sceneName);
		~Scene();

		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) = delete;
		
		/**
		 * \brief	��ʼ������
		 * \param	pNode		���ýڵ�ָ��
		 */
		void Initialize(const XmlNode* pNode);

		/**
		 * \brief	����
		 */
		void Update();

		/**
		 * \brief	���ٳ���
		 */
		void Destroy();
		
		/**
		 * \brief	�Ƿ��ǻ״̬
		 * \return	�״̬
		 */
		bool IsActive() const
		{
			return m_IsActive;
		}
		
		/**
		 * \brief	���û״̬
		 * \param	active		�Ƿ�ʱ�״̬
		 */
		void SetActive(bool active);

		/**
		 * \brief	��ȡ������Ϸ����
		 * \return	��Ϸ����
		 */
		const std::vector<GameObject*>& GetGameObjects() const
		{
			return m_GameObjects;
		}

		/**
		 * \brief	��ȡ������Ϸ����
		 * \return	��Ϸ����
		 */
		std::vector<GameObject*>& GetGameObjects()
		{
			return m_GameObjects;
		}
		
	private:
		/**
		 * \brief	�����Ϸ����
		 * \param	pGameObject	��Ϸ����ָ��
		 */
		void AddGameObject(GameObject* pGameObject);

		/**
		 * \brief	ɾ����Ϸ����
		 * \param	pGameObject	��Ϸ����ָ��
		 */
		void RemoveGameObject(GameObject* pGameObject);

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
		std::vector<GameObject*> m_GameObjects;
	};
}
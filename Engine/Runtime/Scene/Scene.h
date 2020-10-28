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
		GameObjectPtr CreateGameObject();
		
		/**
		 * \brief ����GameObject
		 * \param name GameObject����
		 * \return GameObject
		 */
		GameObjectPtr CreateGameObject(const std::string& name);

		/**
		 * \brief ����GameObject
		 * \param name GameObject����
		 * \return GameObject
		 */
		GameObjectPtr CreateGameObject(std::string&& name);

		/**
		 * \brief ��ȡ����
		 * \param name ��������
		 * \return ����ֻ��ָ��
		 */
		GameObjectPtr GetGameObject(const std::string& name);

		/**
		 * \brief ��ȡ������Ϸ����
		 * \return ��Ϸ����
		 */
		const std::vector<GameObjectPtr>& GetGameObjects() const
		{
			return m_GameObjects;
		}

		/**
		 * \brief ��ȡ������Ϸ����
		 * \return ��Ϸ����
		 */
		std::vector<GameObjectPtr>& GetGameObjects()
		{
			return m_GameObjects;
		}

		/**
		 * \brief �����Ⱦ��
		 * \param renderItem rendererָ��
		 */
		void AddRenderItem(Renderer* renderItem)
		{
			m_RenderItems.push_back(renderItem);
		}

		/**
		 * \brief ��ȡ������Ⱦ��
		 * \return ������Ⱦ��
		 */
		const std::vector<Renderer*>& GetRenderItems() const
		{
			return m_RenderItems;
		}
		
		/**
		 * \brief ɾ����Ⱦ��
		 * \param renderItem rendererָ��
		 */
		void RemoveRenderItem(Renderer* renderItem)
		{
			m_RenderItems.erase(std::find(m_RenderItems.begin(), m_RenderItems.end(), renderItem));
		}

	private:
		/**
		 * \brief ����GameObject
		 * \return GameObject
		 */
		GameObjectPtr CreateGameObjectInternal();
		
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
		std::vector<GameObjectPtr> m_GameObjects;

		/**
		 * \brief ��Ⱦ���б�
		 */
		std::vector<Renderer*> m_RenderItems;
	};
}
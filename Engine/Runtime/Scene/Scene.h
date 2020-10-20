#pragma once

#include "GameObject/GameObject.h"

namespace SaplingEngine
{
	class Scene
	{
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
		 * \brief ����
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
	};
}
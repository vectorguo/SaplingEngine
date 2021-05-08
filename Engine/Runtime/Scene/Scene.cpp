#include "Scene.h"

namespace SaplingEngine
{
	
	/**
	 * \brief GameObjectȫ��ID
	 */
	static uint32_t GameObjectId = 0;
	
	/**
	 * \brief ���캯��
	 * \param sceneName ��������
	 */
	Scene::Scene(const std::string& sceneName) : m_SceneName(sceneName), m_IsActive(false)
	{
		 
	}

	Scene::~Scene() = default;

	/**
	 * \brief ��ʼ������
	 * \param pNode ���ýڵ�ָ��
	 */
	void Scene::Initialize(const XmlNode* pNode)
	{
		for (auto* pChild = pNode->first_node(); pChild != nullptr; pChild = pChild->next_sibling())
		{
			auto gameObject = CreateGameObject();
			gameObject->Deserialize(pChild);
		}
	}

	/**
	 * \brief ����
	 */
	void Scene::Update()
	{
	}

	/**
	 * \brief ���ٳ���
	 */
	void Scene::Destroy()
	{
		//���ٳ����еĶ���
		for (auto iter = m_GameObjects.begin(); iter != m_GameObjects.end(); ++iter)
		{
			auto* pGameObject = *iter;
			pGameObject->m_ScenePtr = nullptr;
			DestroyGameObject(pGameObject);
		}
		m_GameObjects.clear();
	}

	/**
	 * \brief ���û״̬
	 * \param active �Ƿ�ʱ�״̬
	 */
	void Scene::SetActive(bool active)
	{
		m_IsActive = active;
		if (m_IsActive)
		{
			//TODO
		}
		else
		{
			//TODO
		}
	}
	
	/**
	 * \brief	�����Ϸ����
	 * \param	pGameObject	��Ϸ����ָ��
	 */
	void Scene::AddGameObject(GameObject* pGameObject)
	{
#if _DEBUG
		auto iter = std::find(m_GameObjects.begin(), m_GameObjects.end(), pGameObject);
		if (iter == m_GameObjects.end())
		{
			m_GameObjects.push_back(pGameObject);
		}
		else
		{
			throw Exception("�ظ���Scene�����ͬ�Ķ���");
		}
#else
		m_GameObjects.push_back(pGameObject);
#endif
	}
	
	/**
	 * \brief	ɾ����Ϸ����
	 * \param	pGameObject	��Ϸ����ָ��
	 */
	void Scene::RemoveGameObject(GameObject* pGameObject)
	{
		m_GameObjects.erase(std::find(m_GameObjects.begin(), m_GameObjects.end(), pGameObject));
	}
}

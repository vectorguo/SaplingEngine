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

	Scene::~Scene()
	{
		
	}

	/**
	 * \brief ��ʼ������
	 * \param pNode ���ýڵ�ָ��
	 */
	void Scene::Initialize(const XmlNode* pNode)
	{
		for (auto* pChild = pNode->first_node(); pChild != nullptr; pChild = pChild->next_sibling())
		{
			auto gameObject = std::make_shared<GameObject>(GameObjectId++);
			gameObject->Deserialize(pChild);
			m_GameObjects.push_back(gameObject);
		}
	}

	/**
	 * \brief ����
	 */
	void Scene::Update()
	{
		for (auto iter = m_GameObjects.begin(); iter != m_GameObjects.end(); ++iter)
		{
			(*iter)->Update();
		}
	}

	/**
	 * \brief ����
	 */
	void Scene::Destroy()
	{
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
	 * \brief ����GameObject
	 * \return GameObject
	 */
	GameObjectPtr Scene::CreateGameObject()
	{
		auto gameObject = std::make_shared<GameObject>(GameObjectId++);
		gameObject->Initialize();
		m_GameObjects.push_back(gameObject);
		return gameObject;
	}

	/**
	 * \brief ����GameObject
	 * \param name GameObject����
	 * \return GameObject
	 */
	GameObjectPtr Scene::CreateGameObject(const std::string& name)
	{
		auto gameObject = std::make_shared<GameObject>(GameObjectId++, name);
		gameObject->Initialize();
		m_GameObjects.push_back(gameObject);
		return gameObject;
	}

	/**
	 * \brief ����GameObject
	 * \param name GameObject����
	 * \return GameObject
	 */
	GameObjectPtr Scene::CreateGameObject(std::string&& name)
	{
		auto gameObject = std::make_shared<GameObject>(GameObjectId++, std::move(name));
		gameObject->Initialize();
		m_GameObjects.push_back(gameObject);
		return gameObject;
	}

	/**
	 * \brief ����GameObject
	 * \return GameObject
	 */
	GameObjectPtr Scene::CreateGameObjectInternal()
	{
		auto gameObject = std::make_shared<GameObject>(GameObjectId++);
		m_GameObjects.push_back(gameObject);
		return gameObject;
	}
}

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
			auto gameObject = std::make_shared<GameObject>(GameObjectId++);
			gameObject->Deserialize(pChild);
			gameObject->Initialize(this, true);
			m_GameObjects.push_back(gameObject);
		}
	}

	/**
	 * \brief ����
	 */
	void Scene::Update()
	{
		if (!m_NewGameObjects.empty())
		{
			m_GameObjects.insert(m_GameObjects.cend(), m_NewGameObjects.cbegin(), m_NewGameObjects.cend());
			m_NewGameObjects.clear();
		}

		for (auto iter = m_GameObjects.begin(); iter != m_GameObjects.end();)
		{
			auto& pObject = *iter;
			if (pObject->IsDestroyed())
			{
				iter = m_GameObjects.erase(iter);
			}
			else
			{
				pObject->Update();
				++iter;
			}
		}
	}

	/**
	 * \brief ���ٳ���
	 */
	void Scene::Destroy()
	{
		//���ٳ����еĶ���
		for (auto iter = m_GameObjects.begin(); iter != m_GameObjects.end(); ++iter)
		{
			(*iter)->Destroy();
		}
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
	GameObjectSptr Scene::CreateGameObject()
	{
		auto gameObject = std::make_shared<GameObject>(GameObjectId++);
		gameObject->Initialize(this);
		m_NewGameObjects.push_back(gameObject);
		return gameObject;
	}

	/**
	 * \brief ����GameObject
	 * \param name GameObject����
	 * \return GameObject
	 */
	GameObjectSptr Scene::CreateGameObject(const std::string& name)
	{
		auto gameObject = std::make_shared<GameObject>(GameObjectId++, name);
		gameObject->Initialize(this);
		m_NewGameObjects.push_back(gameObject);
		return gameObject;
	}

	/**
	 * \brief ����GameObject
	 * \param name GameObject����
	 * \return GameObject
	 */
	GameObjectSptr Scene::CreateGameObject(std::string&& name)
	{
		auto gameObject = std::make_shared<GameObject>(GameObjectId++, std::move(name));
		gameObject->Initialize(this);
		m_NewGameObjects.push_back(gameObject);
		return gameObject;
	}

	/**
	 * \brief ��ȡ����
	 * \param name ��������
	 * \return ����ָ��
	 */
	GameObject* Scene::GetGameObject(const std::string& name)
	{
		const auto iter = std::find_if(m_GameObjects.begin(), m_GameObjects.end(), [&name](const GameObjectSptr& pObject)
			{
				return pObject->GetName() == name;
			});
		return iter == m_GameObjects.end() ? nullptr : iter->get();
	}

	/**
	 * \brief ��ȡ����
	 * \param name ��������
	 * \return ����ֻ��ָ��
	 */
	GameObjectSptr Scene::GetGameObjectSptr(const std::string& name)
	{
		const auto iter = std::find_if(m_GameObjects.begin(), m_GameObjects.end(), [&name](const GameObjectSptr& pObject)
			{
				return pObject->GetName() == name;
			});
		return iter == m_GameObjects.end() ? nullptr : *iter;
	}

	/**
	 * \brief ����GameObject
	 * \return GameObject
	 */
	GameObjectSptr Scene::CreateGameObjectInternal()
	{
		auto gameObject = std::make_shared<GameObject>(GameObjectId++);
		m_GameObjects.push_back(gameObject);
		return gameObject;
	}
}

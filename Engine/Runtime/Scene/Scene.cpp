#include "Scene.h"

namespace SaplingEngine
{
	
	/**
	 * \brief GameObject全局ID
	 */
	static uint32_t GameObjectId = 0;
	
	/**
	 * \brief 构造函数
	 * \param sceneName 场景名称
	 */
	Scene::Scene(const std::string& sceneName) : m_SceneName(sceneName), m_IsActive(false)
	{
		 
	}

	Scene::~Scene() = default;

	/**
	 * \brief 初始化场景
	 * \param pNode 配置节点指针
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
	 * \brief 更新
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
	 * \brief 销毁场景
	 */
	void Scene::Destroy()
	{
		//销毁场景中的对象
		for (auto iter = m_GameObjects.begin(); iter != m_GameObjects.end(); ++iter)
		{
			(*iter)->Destroy();
		}
	}

	/**
	 * \brief 设置活动状态
	 * \param active 是否时活动状态
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
	 * \brief 创建GameObject
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
	 * \brief 创建GameObject
	 * \param name GameObject名称
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
	 * \brief 创建GameObject
	 * \param name GameObject名称
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
	 * \brief 获取对象
	 * \param name 对象名称
	 * \return 对象指针
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
	 * \brief 获取对象
	 * \param name 对象名称
	 * \return 对象只能指针
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
	 * \brief 创建GameObject
	 * \return GameObject
	 */
	GameObjectSptr Scene::CreateGameObjectInternal()
	{
		auto gameObject = std::make_shared<GameObject>(GameObjectId++);
		m_GameObjects.push_back(gameObject);
		return gameObject;
	}
}

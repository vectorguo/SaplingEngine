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

	Scene::~Scene()
	{
		
	}

	/**
	 * \brief 初始化场景
	 * \param pNode 配置节点指针
	 */
	void Scene::Initialize(const XmlNode* pNode)
	{
		for (auto* pChild = pNode->first_node(); pChild != nullptr; pChild = pChild->next_sibling())
		{
			auto gameObject = std::make_shared<GameObject>(GameObjectId);
			gameObject->Deserialize(pChild, nullptr);
			m_GameObjects.push_back(gameObject);
		}
	}

	/**
	 * \brief 更新
	 */
	void Scene::Update()
	{
		for (auto iter = m_GameObjects.begin(); iter != m_GameObjects.end(); ++iter)
		{
			(*iter)->Update();
		}
	}

	/**
	 * \brief 销毁
	 */
	void Scene::Destroy()
	{
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
}

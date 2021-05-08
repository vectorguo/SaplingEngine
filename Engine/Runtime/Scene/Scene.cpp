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
			auto gameObject = CreateGameObject();
			gameObject->Deserialize(pChild);
		}
	}

	/**
	 * \brief 更新
	 */
	void Scene::Update()
	{
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
}

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
		//环境光颜色
		m_AmbientLightColor.Set(
			XmlGetAttributeValue<float>(pNode, "ambient_r"),
			XmlGetAttributeValue<float>(pNode, "ambient_g"),
			XmlGetAttributeValue<float>(pNode, "ambient_b"),
			XmlGetAttributeValue<float>(pNode, "ambient_a"));

		//节点
		for (auto* pChild = pNode->first_node(); pChild != nullptr; pChild = pChild->next_sibling())
		{
			auto gameObject = CreateGameObject();
			gameObject->Deserialize(pChild);
		}

		m_SceneBounds.Center = XMFLOAT3(0, 0, -120.0f);
		m_SceneBounds.Radius = sqrtf(30.0f * 30.0f);
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
			auto* pGameObject = *iter;
			pGameObject->m_ScenePtr = nullptr;
			DestroyGameObject(pGameObject);
		}
		m_GameObjects.clear();
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
	 * \brief	添加游戏对象
	 * \param	pGameObject	游戏对象指针
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
			throw Exception("重复给Scene添加相同的对象");
		}
#else
		m_GameObjects.push_back(pGameObject);
#endif
	}
	
	/**
	 * \brief	删除游戏对象
	 * \param	pGameObject	游戏对象指针
	 */
	void Scene::RemoveGameObject(GameObject* pGameObject)
	{
		m_GameObjects.erase(std::find(m_GameObjects.begin(), m_GameObjects.end(), pGameObject));
	}
}

#include "Runtime/Application/Setting.h"
#include "Runtime/GameObject/Component.h"
#include "Runtime/GameObject/ComponentFactory.h"
#include "Runtime/GameObject/GameObject.h"
#include "Runtime/GameObject/Transform.h"
#include "Runtime/Scene/Scene.h"
#include "Runtime/Scene/SceneManager.h"

namespace SaplingEngine
{
	ComponentList GameObject::newComponents;
	ComponentList GameObject::destroyedComponents;
	GameObjectList GameObject::newGameObjects;
	GameObjectList GameObject::allGameObjects;

	GameObject::GameObject()
	{
	}

	GameObject::GameObject(const std::string& name) : m_Name(name)
	{
	}

	GameObject::GameObject(std::string&& name) : m_Name(std::move(name))
	{
	}

	GameObject::~GameObject() = default;

	/**
	 * \brief 更新
	 */
	void GameObject::Update()
	{
		for (auto iter = m_Components.begin(); iter != m_Components.end(); ++iter)
		{
			(*iter)->Update();
		}
	}

	/**
	 * \brief 销毁
	 */
	void GameObject::Destroy()
	{
		//从场景中删除自己
		if (m_ScenePtr)
		{
			m_ScenePtr->RemoveGameObject(this);
			m_ScenePtr = nullptr;
		}

		//销毁组件
		for (auto iter = newComponents.begin(); iter != newComponents.end();)
		{
			if ((*iter)->GetGameObject() == this)
			{
				auto* pComponent = iter->Get();
				pComponent->OnDestroy();
				pComponent->m_GameObjectPtr = nullptr;
				pComponent->m_IsDestroyed = true;
				iter = newComponents.erase(iter);
			}
			else
			{
				++iter;
			}
		}

		for (auto iter = destroyedComponents.begin(); iter != destroyedComponents.end();)
		{
			if ((*iter)->GetGameObject() == this)
			{
				iter = destroyedComponents.erase(iter);
			}
			else
			{
				++iter;
			}
		}

		for (auto iter = m_Components.rbegin(); iter != m_Components.rend(); ++iter)
		{
			auto* pComponent = iter->Get();
			pComponent->OnDestroy();
			pComponent->m_GameObjectPtr = nullptr;
			pComponent->m_IsDestroyed = true;
		}
		m_Components.clear();
	}

	/**
	 * \brief 设置活动状态
	 * \param active 是否处于活动状态
	 */
	void GameObject::SetActive(bool active)
	{
		if (m_IsActive != active)
		{
			m_IsActive = active;

			if (active)
			{
				for (auto iter = m_Components.begin(); iter != m_Components.end(); ++iter)
				{
					if ((*iter)->IsEnabled())
					{
						(*iter)->OnEnable();
					}
				}
			}
			else
			{
				for (auto iter = m_Components.begin(); iter != m_Components.end(); ++iter)
				{
					if ((*iter)->IsEnabled())
					{
						(*iter)->OnDisable();
					}
				}
			}
		}
	}

	/**
	 * \brief	更新所有GameObjects
	 */
	void GameObject::UpdateAll()
	{
		//处理新创建的组件
		if (!newComponents.empty())
		{
			static ComponentList tempComponents;

			tempComponents.insert(tempComponents.end(), newComponents.begin(), newComponents.end());
			newComponents.clear();

			//将新创建的组件添加到GameObject的组件列表中
			for (auto iter = tempComponents.begin(); iter != tempComponents.end(); ++iter)
			{
				auto* pGameObject = (*iter)->GetGameObject();
				pGameObject->m_Components.emplace_back(*iter);
			}

			for (auto iter = tempComponents.begin(); iter != tempComponents.end(); ++iter)
			{
				(*iter)->Start();
				(*iter)->OnEnable();
			}

			tempComponents.clear();
		}

		//处理新创建的GameObjects
		if (!newGameObjects.empty())
		{
			allGameObjects.insert(allGameObjects.end(), newGameObjects.begin(), newGameObjects.end());
			newGameObjects.clear();
		}

		//更新所有GameObjects
		for (auto iter = allGameObjects.begin(); iter != allGameObjects.end();)
		{
			auto* pGameObject = iter->Get();
			if (pGameObject->m_IsDestroyed)
			{
				pGameObject->Destroy();
				iter = allGameObjects.erase(iter);
			}
			else
			{
				pGameObject->Update();
				++iter;
			}
		}

		//处理要销毁的组件
		if (!destroyedComponents.empty())
		{
			for (auto iter1 = destroyedComponents.begin(); iter1 != destroyedComponents.end(); ++iter1)
			{
				auto pGameObject = (*iter1)->GetGameObject();
				auto iter2 = std::find(pGameObject->m_Components.begin(), pGameObject->m_Components.end(), *iter1);
				if (iter2 == pGameObject->m_Components.end())
				{
					throw Exception("***");
				}
				else
				{
					auto* pComponent = iter2->Get();
					pComponent->OnDestroy();
					pComponent->m_GameObjectPtr = nullptr;
					pComponent->m_IsDestroyed = true;
					pGameObject->m_Components.erase(iter2);
				}
			}
			destroyedComponents.clear();
		}
	}

	/**
	 * \brief 反序列化
	 * \param pNode 配置节点指针
	 * \return 反序列化是否成功
	 */
	bool GameObject::Deserialize(const XmlNode* pNode)
	{
		m_Name		= XmlGetAttributeValue<const char*>(pNode, "name");
		m_LayerMask = XmlGetAttributeValue<int32_t>(pNode, "layerMask");
		m_IsActive	= XmlGetAttributeValue<bool>(pNode, "isActive");
		m_IsDestroyed = false;

		//序列化Component
		auto* pCmpNodes = pNode->first_node("components");
		if (pCmpNodes)
		{
			for (const auto* pCmpNode = pCmpNodes->first_node(); pCmpNode; pCmpNode = pCmpNode->next_sibling())
			{
				const auto componentType = XmlGetAttributeValue<uint32_t>(pCmpNode, "type");
				const auto componentSubType = XmlGetAttributeValue<uint32_t>(pCmpNode, "subType");
				if (componentType == ComponentType_Transform)
				{
					m_TransformPtr->Deserialize(pCmpNode);
					m_TransformPtr->Awake();
				}
				else
				{
					auto componentSptr = ComponentFactory::CreateComponent(componentType, componentSubType);
#if SAPLING_EDITOR
					if (componentSptr != nullptr)
					{
						newComponents.emplace_back(componentSptr);
						componentSptr->m_GameObjectPtr = this;
						componentSptr->Deserialize(pCmpNode);
						componentSptr->Awake();
					}
#else
					newComponents.emplace_back(componentSptr);
					componentSptr->m_GameObjectPtr = this;
					componentSptr->Deserialize(pCmpNode);
					componentSptr->Awake();
#endif
				}
			}
		}
		else
		{
			//一个组件都没有，反序列化失败
			return false;
		}

		//序列化子节点
		auto* pChildNodes = pNode->first_node("children");
		if (pChildNodes)
		{
			for (const auto* pChildNode = pChildNodes->first_node(); pChildNode; pChildNode = pChildNode->next_sibling())
			{
				auto childObject = CreateGameObject();
				childObject->m_TransformPtr->SetParent(m_TransformPtr);
				childObject->Deserialize(pChildNode);
			}
		}
		
		return true;
	}

	/**
	 * \brief 序列化
	 */
	void GameObject::Serialize()
	{

	}

	/**
	 * \brief 初始化
	 */
	void GameObject::Initialize()
	{
		//创建Transform
		auto transformSptr = AddComponent<Transform>();
		m_TransformPtr = transformSptr.Get();
	}

	/**
	 * \brief	设置对象被销毁
	 */
	void GameObject::SetDestroyed()
	{
		if (m_IsDestroyed)
		{
			return;
		}

		m_IsDestroyed = true;

		//销毁所有子节点
		auto& children = m_TransformPtr->m_Children;
		if (!children.empty())
		{
			for (auto iter = children.begin(); iter != children.end(); ++iter)
			{
				auto pChild = (*iter)->m_GameObjectPtr;
				pChild->SetDestroyed();
			}
		}
	}

	/**
	 * \brief	创建GameObject
	 * \return	GameObject智能指针
	 */
	GameObjectSptr CreateGameObject()
	{
		auto gameObject = MakeShared<GameObject>();
		gameObject->Initialize();
		GameObject::newGameObjects.emplace_back(gameObject);

		//添加到活动场景
		auto* pActiveScene = SceneManager::GetActiveScene();
		pActiveScene->AddGameObject(gameObject.Get());
		gameObject->m_ScenePtr = pActiveScene;

		return gameObject;
	}
	
	/**
	 * \brief	创建GameObject
	 * \param	name			GameObject名称
	 * \return	GameObject智能指针
	 */
	GameObjectSptr CreateGameObject(const std::string& name)
	{
		auto gameObject = MakeShared<GameObject>(name);
		gameObject->Initialize();
		GameObject::newGameObjects.emplace_back(gameObject);

		//添加到活动场景
		auto* pActiveScene = SceneManager::GetActiveScene();
		pActiveScene->AddGameObject(gameObject.Get());
		gameObject->m_ScenePtr = pActiveScene;

		return gameObject;
	}

	/**
	 * \brief	创建GameObject
	 * \param	name			GameObject名称
	 * \return	GameObject智能指针
	 */
	GameObjectSptr CreateGameObject(std::string&& name)
	{
		auto gameObject = MakeShared<GameObject>(std::move(name));
		gameObject->Initialize();
		GameObject::newGameObjects.emplace_back(gameObject);

		//添加到活动场景
		auto* pActiveScene = SceneManager::GetActiveScene();
		pActiveScene->AddGameObject(gameObject.Get());
		gameObject->m_ScenePtr = pActiveScene;

		return gameObject;
	}

	/**
	 * \brief	查找GameObject
	 * \param	name			GameObject名称
	 * \return	GameObject智能指针
	 */
	GameObjectSptr FindGameObject(const std::string& name)
	{
		auto iter = std::find_if(GameObject::allGameObjects.begin(), GameObject::allGameObjects.end(),
			[&name](const GameObjectSptr& gameObjectSptr)
			{
				return gameObjectSptr->GetName() == name;
			});
		if (iter == GameObject::allGameObjects.end())
		{
			iter = std::find_if(GameObject::newGameObjects.begin(), GameObject::newGameObjects.end(),
				[&name](const GameObjectSptr& gameObjectSptr)
				{
					return gameObjectSptr->GetName() == name;
				});
			return iter == GameObject::newGameObjects.end() ? nullptr : *iter;
		}
		else
		{
			return *iter;
		}
	}
}

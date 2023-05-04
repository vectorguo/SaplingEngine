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
	 * \brief ����
	 */
	void GameObject::Update()
	{
		for (auto iter = m_Components.begin(); iter != m_Components.end(); ++iter)
		{
			(*iter)->Update();
		}
	}

	/**
	 * \brief ����
	 */
	void GameObject::Destroy()
	{
		//�ӳ�����ɾ���Լ�
		if (m_ScenePtr)
		{
			m_ScenePtr->RemoveGameObject(this);
			m_ScenePtr = nullptr;
		}

		//�������
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
	 * \brief ���û״̬
	 * \param active �Ƿ��ڻ״̬
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
	 * \brief	��������GameObjects
	 */
	void GameObject::UpdateAll()
	{
		//�����´��������
		if (!newComponents.empty())
		{
			static ComponentList tempComponents;

			tempComponents.insert(tempComponents.end(), newComponents.begin(), newComponents.end());
			newComponents.clear();

			//���´����������ӵ�GameObject������б���
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

		//�����´�����GameObjects
		if (!newGameObjects.empty())
		{
			allGameObjects.insert(allGameObjects.end(), newGameObjects.begin(), newGameObjects.end());
			newGameObjects.clear();
		}

		//��������GameObjects
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

		//����Ҫ���ٵ����
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
	 * \brief �����л�
	 * \param pNode ���ýڵ�ָ��
	 * \return �����л��Ƿ�ɹ�
	 */
	bool GameObject::Deserialize(const XmlNode* pNode)
	{
		m_Name		= XmlGetAttributeValue<const char*>(pNode, "name");
		m_LayerMask = XmlGetAttributeValue<int32_t>(pNode, "layerMask");
		m_IsActive	= XmlGetAttributeValue<bool>(pNode, "isActive");
		m_IsDestroyed = false;

		//���л�Component
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
			//һ�������û�У������л�ʧ��
			return false;
		}

		//���л��ӽڵ�
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
	 * \brief ���л�
	 */
	void GameObject::Serialize()
	{

	}

	/**
	 * \brief ��ʼ��
	 */
	void GameObject::Initialize()
	{
		//����Transform
		auto transformSptr = AddComponent<Transform>();
		m_TransformPtr = transformSptr.Get();
	}

	/**
	 * \brief	���ö�������
	 */
	void GameObject::SetDestroyed()
	{
		if (m_IsDestroyed)
		{
			return;
		}

		m_IsDestroyed = true;

		//���������ӽڵ�
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
	 * \brief	����GameObject
	 * \return	GameObject����ָ��
	 */
	GameObjectSptr CreateGameObject()
	{
		auto gameObject = MakeShared<GameObject>();
		gameObject->Initialize();
		GameObject::newGameObjects.emplace_back(gameObject);

		//��ӵ������
		auto* pActiveScene = SceneManager::GetActiveScene();
		pActiveScene->AddGameObject(gameObject.Get());
		gameObject->m_ScenePtr = pActiveScene;

		return gameObject;
	}
	
	/**
	 * \brief	����GameObject
	 * \param	name			GameObject����
	 * \return	GameObject����ָ��
	 */
	GameObjectSptr CreateGameObject(const std::string& name)
	{
		auto gameObject = MakeShared<GameObject>(name);
		gameObject->Initialize();
		GameObject::newGameObjects.emplace_back(gameObject);

		//��ӵ������
		auto* pActiveScene = SceneManager::GetActiveScene();
		pActiveScene->AddGameObject(gameObject.Get());
		gameObject->m_ScenePtr = pActiveScene;

		return gameObject;
	}

	/**
	 * \brief	����GameObject
	 * \param	name			GameObject����
	 * \return	GameObject����ָ��
	 */
	GameObjectSptr CreateGameObject(std::string&& name)
	{
		auto gameObject = MakeShared<GameObject>(std::move(name));
		gameObject->Initialize();
		GameObject::newGameObjects.emplace_back(gameObject);

		//��ӵ������
		auto* pActiveScene = SceneManager::GetActiveScene();
		pActiveScene->AddGameObject(gameObject.Get());
		gameObject->m_ScenePtr = pActiveScene;

		return gameObject;
	}

	/**
	 * \brief	����GameObject
	 * \param	name			GameObject����
	 * \return	GameObject����ָ��
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

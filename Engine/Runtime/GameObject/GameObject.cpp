#include "GameObject/Component.h"
#include "GameObject/ComponentFactory.h"
#include "GameObject/GameObject.h"
#include "GameObject/Transform.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

namespace SaplingEngine
{
	ComponentList GameObject::newComponents;
	ComponentList GameObject::destroyedComponents;

	GameObject::GameObject(uint32_t id) : m_Id(id)
	{
	}

	GameObject::GameObject(uint32_t id, const std::string& name) : m_Id(id), m_Name(name)
	{
	}

	GameObject::GameObject(uint32_t id, std::string&& name) : m_Id(id), m_Name(std::move(name))
	{
	}

	GameObject::~GameObject() = default;

	/**
	 * \brief	�����´��������
	 */
	void GameObject::HandleNewComponents()
	{
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
	}

	/**
	 * \brief	����Ҫ���ٵ����
	 */
	void GameObject::HandleDestroyedComponents()
	{
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
	 * \brief ��ʼ��
	 * \param pScene ��������
	 * \param isDeserialized �Ƿ�ʱ�����л���GameObject��ʼ��
	 * \return �Ƿ��ʼ���ɹ�
	 */
	bool GameObject::Initialize(Scene* pScene, bool isDeserialized)
	{
		m_pScene = pScene;
		if (isDeserialized)
		{
			
		}
		else
		{
			m_TransformSptr = AddComponent<Transform>();
		}
		return true;
	}

	/**
	 * \brief ����
	 */
	void GameObject::Update()
	{
		//�������
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
		if (m_Parent != nullptr)
		{
			m_Parent->m_Children.erase(std::find_if(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), [this](const GameObjectSptr& pChild)
			{
				return pChild.Get() == this;
			}));
			m_Parent = nullptr;
		}

		//�����ӽڵ�
		DestroyInternal();
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
	 * \brief ����parent
	 * \param parent parent
	 */
	void GameObject::SetParent(const GameObjectSptr& parent)
	{
		if (m_Parent == parent)
		{
			return;
		}

		if (m_Parent != nullptr)
		{
			m_Parent->m_Children.erase(std::find_if(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), [this](const GameObjectSptr& pChild)
			{
				return pChild.Get() == this;
			}));
		}

		m_Parent = parent;

		if (m_Parent != nullptr)
		{
			m_Parent->m_Children.push_back(shared_from_this());
		}
	}

	/**
	 * \brief ���л�
	 */
	void GameObject::Serialize()
	{
		
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
				auto* pComponent = ComponentFactory::CreateComponent(componentType, componentSubType);
				pComponent->Deserialize(pCmpNode);
				AddComponent(componentType, pComponent);
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
			auto* pActiveScene = SceneManager::GetActiveScene();
			for (const auto* pChildNode = pChildNodes->first_node(); pChildNode; pChildNode = pChildNode->next_sibling())
			{
				auto pChild = pActiveScene->CreateGameObjectInternal();
				pChild->Deserialize(pChildNode);
				pChild->SetParent(shared_from_this());
			}
		}
		
		return true;
	}

	/**
	 * \brief	��������ֻ�����ͨ��ComponentFactory���������
	 * \param	componentType	�������
	 * \param	pComponent		Ҫ����ӵ����ָ��
	 */
	void GameObject::AddComponent(uint32_t componentType, Component* pComponent)
	{
		newComponents.emplace_back(pComponent);

		pComponent->m_GameObjectPtr = this;
		pComponent->Awake();

		if (componentType == ComponentType_Transform)
		{
			m_TransformSptr = StaticPointerCast<Transform>(*newComponents.rbegin());
		}
	}

	/**
	 * \brief �����ӽڵ�
	 */
	void GameObject::DestroyInternal()
	{
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

		for (auto iter = m_Components.begin(); iter != m_Components.end(); ++iter)
		{
			auto* pComponent = iter->Get();
			pComponent->OnDestroy();
			pComponent->m_GameObjectPtr = nullptr;
			pComponent->m_IsDestroyed = true;
		}
		m_Components.clear();

		for (auto& child : m_Children)
		{
			child->Destroy();
			child->m_Parent = nullptr;
		}
		m_Children.clear();
	}

	/**
	 * \brief ����GameObject
	 * \param gameObject go
	 */
	void DestroyGameObject(const GameObjectSptr& gameObject)
	{
		gameObject->m_IsDestroyed = true;
	}
}

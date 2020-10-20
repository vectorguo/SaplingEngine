#include "GameObject.h"
#include "ComponentFactory.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

namespace SaplingEngine
{
	/**
	 * \brief ����GameObject
	 * \param gameObject go
	 */
	void DestroyGameObject(const GameObjectPtr& gameObject)
	{
		gameObject->m_IsDestroyed = true;
	}
	
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
	 * \brief ��ʼ��
	 * \param isDeserialized �Ƿ�ʱ�����л���GameObject��ʼ��
	 * \return �Ƿ��ʼ���ɹ�
	 */
	bool GameObject::Initialize(bool isDeserialized)
	{
		if (isDeserialized)
		{
			
		}
		else
		{
			m_Transform = AddComponent<Transform>();
		}
		return true;
	}

	/**
	 * \brief ����
	 */
	void GameObject::Update()
	{
		//����µ����
		if (!m_NewComponents.empty())
		{
			for (auto iter = m_NewComponents.begin(); iter != m_NewComponents.end(); ++iter)
			{
				m_Components.insert_or_assign(iter->first, iter->second);
			}
			
			for (auto iter = m_NewComponents.begin(); iter != m_NewComponents.end(); ++iter)
			{
				iter->second->Start();
				iter->second->OnEnable();
			}
			
			m_NewComponents.clear();
		}

		//ɾ�����
		if (!m_DestroyedComponents.empty())
		{
			for (auto componentType : m_DestroyedComponents)
			{
				auto iter = m_Components.find(componentType);
				if (iter != m_Components.end())
				{
					iter->second->OnDestroy();
					m_Components.erase(iter);
				}
			}
			m_DestroyedComponents.clear();
		}

		//�������
		for (auto iter = m_Components.begin(); iter != m_Components.end(); ++iter)
		{
			iter->second->Update();
		}
	}

	/**
	 * \brief ����
	 */
	void GameObject::Destroy()
	{
		if (m_Parent != nullptr)
		{
			m_Parent->m_Children.erase(std::find_if(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), [this](const GameObjectPtr& pChild)
			{
				return pChild.get() == this;
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
					if (iter->second->IsEnabled())
					{
						iter->second->OnEnable();
					}
				}
			}
			else
			{
				for (auto iter = m_Components.begin(); iter != m_Components.end(); ++iter)
				{
					if (iter->second->IsEnabled())
					{
						iter->second->OnDisable();
					}
				}
			}
		}
	}

	/**
	 * \brief ����parent
	 * \param parent parent
	 */
	void GameObject::SetParent(const GameObjectPtr& parent)
	{
		if (m_Parent == parent)
		{
			return;
		}

		if (m_Parent != nullptr)
		{
			m_Parent->m_Children.erase(std::find_if(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), [this](const GameObjectPtr& pChild)
			{
				return pChild.get() == this;
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
				auto* pComponent = ComponentFactory::Instance()->CreateComponent(componentType, componentSubType);
				AddComponent(componentType, pComponent);
				pComponent->Deserialize(pCmpNode);
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
			auto* pActiveScene = SceneManager::Instance()->GetActiveScene();
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
	 * \brief ��������ֻ�����ͨ��ComponentFactory���������
	 * \param componentType �������
	 * \param pComponent Ҫ����ӵ����ָ��
	 */
	void GameObject::AddComponent(uint32_t componentType, Component* pComponent)
	{
		if (m_NewComponents.find(componentType) == m_NewComponents.end() && m_Components.find(componentType) == m_Components.end())
		{
			//û�������ͬ���͵����
			std::shared_ptr<Component> componentPtr(pComponent);
			componentPtr->SetOwner(shared_from_this());
			m_NewComponents.insert_or_assign(componentType, componentPtr);
			m_NewComponents[componentType]->Awake();
		}
	}

	/**
	 * \brief �����ӽڵ�
	 */
	void GameObject::DestroyInternal()
	{
		//�������
		for (auto iter = m_NewComponents.begin(); iter != m_NewComponents.end(); ++iter)
		{
			iter->second->OnDestroy();
			iter->second->m_pOwner = nullptr;
		}
		m_NewComponents.clear();

		for (auto iter = m_Components.begin(); iter != m_Components.end(); ++iter)
		{
			iter->second->OnDestroy();
			iter->second->m_pOwner = nullptr;
		}
		m_Components.clear();

		for (auto& child : m_Children)
		{
			child->Destroy();
			child->m_Parent = nullptr;
		}
		m_Children.clear();
	}
}

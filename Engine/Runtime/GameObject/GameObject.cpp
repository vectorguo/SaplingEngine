#include "GameObject/GameObject.h"

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

	/**
	 * \brief ��ʼ��
	 * \return �Ƿ��ʼ���ɹ�
	 */
	bool GameObject::Initialize()
	{
		m_Transform = AddComponent<Transform>();
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

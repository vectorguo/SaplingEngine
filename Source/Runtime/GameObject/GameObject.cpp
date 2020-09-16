#include "GameObject/GameObject.h"

namespace SaplingEngine
{
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
		//���Transform���
		m_pTransform = AddComponent<Transform>();

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
			}
			
			m_NewComponents.clear();
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
		for (auto iter = m_Components.begin(); iter != m_Components.end(); ++iter)
		{
			iter->second->m_pOwner.reset();
		}
		m_Components.clear();
		m_pTransform.reset();
	}

	/**
	 * \brief ���û״̬
	 * \param active �Ƿ��ڻ״̬
	 */
	void GameObject::SetActive(bool active)
	{
		
	}
}

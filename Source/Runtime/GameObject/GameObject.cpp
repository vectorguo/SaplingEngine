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
	 * \brief 初始化
	 * \return 是否初始化成功
	 */
	bool GameObject::Initialize()
	{
		//添加Transform组件
		m_pTransform = AddComponent<Transform>();

		return true;
	}

	/**
	 * \brief 更新
	 */
	void GameObject::Update()
	{
		//添加新的组件
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

		//更新组件
		for (auto iter = m_Components.begin(); iter != m_Components.end(); ++iter)
		{
			iter->second->Update();
		}
	}

	/**
	 * \brief 销毁
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
	 * \brief 设置活动状态
	 * \param active 是否处于活动状态
	 */
	void GameObject::SetActive(bool active)
	{
		
	}
}

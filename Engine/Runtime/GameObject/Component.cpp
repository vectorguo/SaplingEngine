#include "GameObject.h"
#include "GameObject/Component.h"

namespace SaplingEngine
{
	Component::Component() = default;
	Component::~Component() = default;

	Transform* Component::GetTransform() const
	{
		return m_pGameObject->GetTransform();
	}

	TransformPtr& Component::GetTransformPtr() const
	{
		return m_pGameObject->GetTransformPtr();
	}

	/**
	 * \brief 设置活动状态
	 * \param enabled 是否使活动状态
	 */
	void Component::SetEnabled(const bool enabled)
	{
		if (m_IsEnabled != enabled)
		{
			m_IsEnabled = enabled;

			if (enabled)
			{
				OnEnable();
			}
			else
			{
				OnDisable();
			}
		}
	}

	/**
	 * \brief 序列化
	 */
	void Component::Serialize()
	{
		
	}

	/**
	 * \brief 反序列化
	 * \param pNode 配置节点指针
	 * \return 反序列化是否成功
	 */
	bool Component::Deserialize(const XmlNode* pNode)
	{
		return true;
	}
}

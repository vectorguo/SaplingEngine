#include "GameObject.h"
#include "GameObject/Component.h"

namespace SaplingEngine
{
	/**
	 * \brief	构造函数
	 */
	Component::Component(uint32_t componentType) : m_ComponentType(componentType)
	{

	}

	Component::~Component() = default;

	Transform* Component::GetTransform() const
	{
		return m_GameObjectPtr->GetTransform();
	}

	TransformSptr Component::GetTransformSptr() const
	{
		return m_GameObjectPtr->GetTransformSptr();
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
	 * \brief 反序列化
	 * \param pNode 配置节点指针
	 * \return 反序列化是否成功
	 */
	bool Component::Deserialize(const XmlNode* pNode)
	{
		return true;
	}

	/**
	 * \brief 序列化
	 */
	void Component::Serialize()
	{

	}
}

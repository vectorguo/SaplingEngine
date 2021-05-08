#include "GameObject.h"
#include "GameObject/Component.h"

namespace SaplingEngine
{
	/**
	 * \brief	���캯��
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
	 * \brief ���û״̬
	 * \param enabled �Ƿ�ʹ�״̬
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
	 * \brief �����л�
	 * \param pNode ���ýڵ�ָ��
	 * \return �����л��Ƿ�ɹ�
	 */
	bool Component::Deserialize(const XmlNode* pNode)
	{
		return true;
	}

	/**
	 * \brief ���л�
	 */
	void Component::Serialize()
	{

	}
}

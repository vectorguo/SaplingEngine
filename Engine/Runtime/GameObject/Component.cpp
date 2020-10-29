#include "GameObject.h"
#include "GameObject/Component.h"

namespace SaplingEngine
{
	Component::Component() = default;
	Component::~Component() = default;

	Transform* Component::GetTransform() const
	{
		return m_GameObjectSptr->GetTransform();
	}

	TransformSptr& Component::GetTransformSptr() const
	{
		return m_GameObjectSptr->GetTransformSptr();
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
	 * \brief ���л�
	 */
	void Component::Serialize()
	{
		
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
}

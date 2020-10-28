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

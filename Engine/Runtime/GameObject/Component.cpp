#include "GameObject/Component.h"

namespace SaplingEngine
{
	Component::Component() = default;
	Component::~Component() = default;

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

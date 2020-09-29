#include "GameObject/Component.h"

namespace SaplingEngine
{
	Component::Component() = default;
	Component::~Component() = default;

	/**
	 * \brief ���û״̬
	 * \param enabled �Ƿ�ʹ�״̬
	 */
	void Component::SetEnabled(bool enabled)
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
}

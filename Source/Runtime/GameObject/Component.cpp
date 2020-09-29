#include "GameObject/Component.h"

namespace SaplingEngine
{
	Component::Component() = default;
	Component::~Component() = default;

	/**
	 * \brief 设置活动状态
	 * \param enabled 是否使活动状态
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

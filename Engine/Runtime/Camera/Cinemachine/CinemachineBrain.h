#pragma once

#include "GameObject/Component.h"

namespace SaplingEngine
{
	class CinemachineBrain final : Component
	{
	public:
		CinemachineBrain() = default;
		~CinemachineBrain() override = default;

		/**
		 * \brief	禁止拷贝和移动
		 */
		CinemachineBrain(const CinemachineBrain&) = delete;
		CinemachineBrain(CinemachineBrain&&) = delete;
		CinemachineBrain& operator= (const CinemachineBrain&) = delete;
		CinemachineBrain& operator= (CinemachineBrain&&) = delete;

		/**
		 * \brief	获取组件类型。
		 *			每个组件都必须定义此函数。
		 *			GameObject对于每个ComponentType只能拥有一个Component。
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_CinemachineBrain;
		}
	};
}
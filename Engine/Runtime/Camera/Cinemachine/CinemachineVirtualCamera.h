#pragma once

#include "GameObject/Component.h"

namespace SaplingEngine
{
	class CinemachineVirtualCamera final : Component
	{
	public:
		CinemachineVirtualCamera() = default;
		~CinemachineVirtualCamera() override = default;

		/**
		 * \brief	禁止拷贝和移动
		 */
		CinemachineVirtualCamera(const CinemachineVirtualCamera&) = delete;
		CinemachineVirtualCamera(CinemachineVirtualCamera&&) = delete;
		CinemachineVirtualCamera& operator= (const CinemachineVirtualCamera&) = delete;
		CinemachineVirtualCamera& operator= (CinemachineVirtualCamera&&) = delete;

		/**
		 * \brief	获取组件类型。
		 *			每个组件都必须定义此函数。
		 *			GameObject对于每个ComponentType只能拥有一个Component。
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_CinemachineVirtualCamera;
		}
	};
}
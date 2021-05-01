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
		 * \brief	��ֹ�������ƶ�
		 */
		CinemachineVirtualCamera(const CinemachineVirtualCamera&) = delete;
		CinemachineVirtualCamera(CinemachineVirtualCamera&&) = delete;
		CinemachineVirtualCamera& operator= (const CinemachineVirtualCamera&) = delete;
		CinemachineVirtualCamera& operator= (CinemachineVirtualCamera&&) = delete;

		/**
		 * \brief	��ȡ������͡�
		 *			ÿ����������붨��˺�����
		 *			GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component��
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_CinemachineVirtualCamera;
		}
	};
}
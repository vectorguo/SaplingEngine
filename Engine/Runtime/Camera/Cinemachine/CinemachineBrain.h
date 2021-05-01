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
		 * \brief	��ֹ�������ƶ�
		 */
		CinemachineBrain(const CinemachineBrain&) = delete;
		CinemachineBrain(CinemachineBrain&&) = delete;
		CinemachineBrain& operator= (const CinemachineBrain&) = delete;
		CinemachineBrain& operator= (CinemachineBrain&&) = delete;

		/**
		 * \brief	��ȡ������͡�
		 *			ÿ����������붨��˺�����
		 *			GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component��
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_CinemachineBrain;
		}
	};
}
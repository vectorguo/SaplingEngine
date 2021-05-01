#pragma once

#include "SaplingEngineHeader.h"
#include "WarcraftEnum.h"

namespace Warcraft
{
	class WarcraftGameMain final : public SaplingEngine::Component
	{
	public:
		WarcraftGameMain() = default;
		~WarcraftGameMain() override = default;

		/**
		 * \brief	��ֹ�������ƶ�
		 */
		WarcraftGameMain(const WarcraftGameMain&) = delete;
		WarcraftGameMain(WarcraftGameMain&&) = delete;
		WarcraftGameMain& operator= (const WarcraftGameMain&) = delete;
		WarcraftGameMain& operator= (WarcraftGameMain&&) = delete;

		/**
		 * \brief	��ȡ������͡�
		 *			ÿ����������붨��˺�����
		 *			GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component��
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_WarcraftGameMain;
		}

		/**
		 * \brief	�����л�
		 * \param	pNode		���ýڵ�ָ��
		 * \return	�����л��Ƿ�ɹ�
		 */
		bool Deserialize(const XmlNode * pNode) override;

	protected:
		/**
		 * \brief	Start
		 */
		void Start() override;

		/**
		 * \brief	����
		 */
		void Update() override;

	};
}
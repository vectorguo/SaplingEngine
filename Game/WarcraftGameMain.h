#pragma once

#include "Game/Character/Character.h"
#include "Game/WarcraftEnum.h"
#include "SaplingEngineHeader.h"

namespace Warcraft
{
	class WarcraftGameMain final : public SaplingEngine::Component
	{
	public:
		WarcraftGameMain() : Component(ComponentType_WarcraftGameMain)
		{

		}
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
		static constexpr uint32_t GetStaticComponentType()
		{
			return ComponentType_WarcraftGameMain;
		}

		/**
		 * \brief	�����л�
		 * \param	pNode		���ýڵ�ָ��
		 * \return	�����л��Ƿ�ɹ�
		 */
		bool Deserialize(const XmlNode * pNode) override;

		/**
		 * \brief	����
		 */
		static WarcraftGameMain* Instance()
		{
			return pInstance;
		}

		/**
		 * \brief	��ȡ��ҽ�ɫָ��
		 */
		Character* GetPlayer() const
		{
			return m_PlayerSptr.Get();
		}

	protected:
		/**
		 * \brief	Awake
		 */
		void Awake() override
		{
			pInstance = this;
		}

		/**
		 * \brief	Start
		 */
		void Start() override;

		/**
		 * \brief	����
		 */
		void Update() override;

	private:
		/**
		 * \brief	����
		 */
		static WarcraftGameMain* pInstance;

		/**
		 * \brief	��ҽ�ɫ������ָ��
		 */
		CharacterSptr m_PlayerSptr;
	};
}
#pragma once

#include "Game/WarcraftEnum.h"
#include "SaplingEngineHeader.h"

namespace Warcraft
{
	class Character final : public SaplingEngine::Component
	{
	public:
		Character() : Component(ComponentType_Character)
		{

		}
		~Character() override = default;

		/**
		 * \brief	��ֹ�������ƶ�
		 */
		Character(const Character&) = delete;
		Character(Character&&) = delete;
		Character& operator= (const Character&) = delete;
		Character& operator= (Character&&) = delete;

		/**
		 * \brief	��ȡ������͡�
		 *			ÿ����������붨��˺�����
		 *			GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component��
		 */
		static constexpr uint32_t GetStaticComponentType()
		{
			return ComponentType_Character;
		}

		/**
		 * \brief	�����л�
		 * \param	pNode		���ýڵ�ָ��
		 * \return	�����л��Ƿ�ɹ�
		 */
		bool Deserialize(const XmlNode* pNode) override;

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

	using CharacterSptr = SharedPtr<Character>;
}
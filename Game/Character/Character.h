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
		 * \brief	禁止拷贝和移动
		 */
		Character(const Character&) = delete;
		Character(Character&&) = delete;
		Character& operator= (const Character&) = delete;
		Character& operator= (Character&&) = delete;

		/**
		 * \brief	获取组件类型。
		 *			每个组件都必须定义此函数。
		 *			GameObject对于每个ComponentType只能拥有一个Component。
		 */
		static constexpr uint32_t GetStaticComponentType()
		{
			return ComponentType_Character;
		}

		/**
		 * \brief	反序列化
		 * \param	pNode		配置节点指针
		 * \return	反序列化是否成功
		 */
		bool Deserialize(const XmlNode* pNode) override;

	protected:
		/**
		 * \brief	Start
		 */
		void Start() override;

		/**
		 * \brief	更新
		 */
		void Update() override;
	};

	using CharacterSptr = SharedPtr<Character>;
}
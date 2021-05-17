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
		 * \brief	禁止拷贝和移动
		 */
		WarcraftGameMain(const WarcraftGameMain&) = delete;
		WarcraftGameMain(WarcraftGameMain&&) = delete;
		WarcraftGameMain& operator= (const WarcraftGameMain&) = delete;
		WarcraftGameMain& operator= (WarcraftGameMain&&) = delete;

		/**
		 * \brief	获取组件类型。
		 *			每个组件都必须定义此函数。
		 *			GameObject对于每个ComponentType只能拥有一个Component。
		 */
		static constexpr uint32_t GetStaticComponentType()
		{
			return ComponentType_WarcraftGameMain;
		}

		/**
		 * \brief	反序列化
		 * \param	pNode		配置节点指针
		 * \return	反序列化是否成功
		 */
		bool Deserialize(const XmlNode * pNode) override;

		/**
		 * \brief	单例
		 */
		static WarcraftGameMain* Instance()
		{
			return pInstance;
		}

		/**
		 * \brief	获取玩家角色指针
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
		 * \brief	更新
		 */
		void Update() override;

	private:
		/**
		 * \brief	单例
		 */
		static WarcraftGameMain* pInstance;

		/**
		 * \brief	玩家角色的智能指针
		 */
		CharacterSptr m_PlayerSptr;
	};
}
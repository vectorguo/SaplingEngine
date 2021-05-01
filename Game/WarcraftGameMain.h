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
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_WarcraftGameMain;
		}

		/**
		 * \brief	反序列化
		 * \param	pNode		配置节点指针
		 * \return	反序列化是否成功
		 */
		bool Deserialize(const XmlNode * pNode) override;

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
}
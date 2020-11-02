#pragma once

#include "GameObject/Component.h"

namespace SaplingEngine
{
	class Light final : public Component
	{
	public:
		Light();
		~Light() override;

		/*
		 * 禁止拷贝和移动
		 */
		Light(const Light&) = delete;
		Light(Light&&) = delete;
		Light& operator= (const Light&) = delete;
		Light& operator= (Light&&) = delete;

		/*
		 * 获取组件类型
		 * 每个组件都必须定义此函数
		 * GameObject对于每个ComponentType只能拥有一个Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_Light;
		}

		/**
		 * \brief 反序列化
		 * \param pNode 配置节点指针
		 * \return 反序列化是否成功
		 */
		bool Deserialize(const XmlNode* pNode) override;

	public:
		/**
		 * \brief 获取单例
		 * \return 单例指针
		 */
		static Light* Instance()
		{
			return m_pInstance;
		}

	private:
		/**
		 * \brief 单例
		 */
		static Light* m_pInstance;
	};
}
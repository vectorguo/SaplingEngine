#pragma once

#include "Runtime/Component/Component.h"

namespace SaplingEngine
{
	/**
	 * \brief	光照类型
	 */
	enum class ELightType : uint8_t
	{
		DirectionalLight,
		PointLight,
		SpotLight,
	};

	class Light final : public Component
	{
	public:
		Light() : Component(ComponentType_Light)
		{

		}
		~Light() override = default;

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
		static constexpr uint32_t GetStaticComponentType()
		{
			return ComponentType_Light;
		}

	public:
		/**
		 * \brief	获取光照类型
		 */
		ELightType GetLightType() const
		{
			return m_LightType;
		}

		/**
		 * \brief	获取光照颜色
		 */
		const Color& GetLightColor() const
		{
			return m_LightColor;
		}

		/**
		 * \brief	获取光照颜色
		 */
		Color& GetLightColor()
		{
			return m_LightColor;
		}

		/**
		 * \brief	获取阴影距离
		 */
		float GetShadowDistance() const
		{
			return m_ShadowDistance;
		}

		/**
		 * \brief	获取阴影强度
		 */
		float GetShadowStrength() const
		{
			return m_ShadowStrength;
		}

		/**
		 * \brief	获取光照方向
		 */
		Vector3 GetLightDirection() const;

		/**
		 * \brief	获取光照位置
		 */
		const Vector3& GetLightPosition() const;

	private:
		/**
		 * \brief 反序列化
		 * \param pNode 配置节点指针
		 * \return 反序列化是否成功
		 */
		bool Deserialize(const XmlNode* pNode) override;

		/**
		 * \brief	Awake
		 */
		void Awake() override;

		/**
		 * \brief	OnDestroy
		 */
		void OnDestroy() override;

	private:
		/**
		 * \brief	光照类型
		 */
		ELightType m_LightType = ELightType::DirectionalLight;

		/**
		 * \brief	光照颜色
		 */
		Color m_LightColor;

		/**
		 * \brief	阴影距离
		 */
		float m_ShadowDistance = 30.0f;

		/**
		 * \brief	阴影强度
		 */
		float m_ShadowStrength = 1.0f;
	};
}
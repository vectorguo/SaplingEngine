#pragma once

#include "Runtime/Component/Component.h"

namespace SaplingEngine
{
	/**
	 * \brief	��������
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
		 * ��ֹ�������ƶ�
		 */
		Light(const Light&) = delete;
		Light(Light&&) = delete;
		Light& operator= (const Light&) = delete;
		Light& operator= (Light&&) = delete;

		/*
		 * ��ȡ�������
		 * ÿ����������붨��˺���
		 * GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component
		 */
		static constexpr uint32_t GetStaticComponentType()
		{
			return ComponentType_Light;
		}

	public:
		/**
		 * \brief	��ȡ��������
		 */
		ELightType GetLightType() const
		{
			return m_LightType;
		}

		/**
		 * \brief	��ȡ������ɫ
		 */
		const Color& GetLightColor() const
		{
			return m_LightColor;
		}

		/**
		 * \brief	��ȡ������ɫ
		 */
		Color& GetLightColor()
		{
			return m_LightColor;
		}

		/**
		 * \brief	��ȡ��Ӱ����
		 */
		float GetShadowDistance() const
		{
			return m_ShadowDistance;
		}

		/**
		 * \brief	��ȡ��Ӱǿ��
		 */
		float GetShadowStrength() const
		{
			return m_ShadowStrength;
		}

		/**
		 * \brief	��ȡ���շ���
		 */
		Vector3 GetLightDirection() const;

		/**
		 * \brief	��ȡ����λ��
		 */
		const Vector3& GetLightPosition() const;

	private:
		/**
		 * \brief �����л�
		 * \param pNode ���ýڵ�ָ��
		 * \return �����л��Ƿ�ɹ�
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
		 * \brief	��������
		 */
		ELightType m_LightType = ELightType::DirectionalLight;

		/**
		 * \brief	������ɫ
		 */
		Color m_LightColor;

		/**
		 * \brief	��Ӱ����
		 */
		float m_ShadowDistance = 30.0f;

		/**
		 * \brief	��Ӱǿ��
		 */
		float m_ShadowStrength = 1.0f;
	};
}
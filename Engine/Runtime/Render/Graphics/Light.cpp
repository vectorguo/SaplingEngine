#include "GameObject/Transform.h"
#include "Render/Graphics/Light.h"
#include "Render/Graphics/LightManager.h"

namespace SaplingEngine
{
	/**
	 * \brief	��ȡ���շ���
	 */
	Vector3 Light::GetLightDirection() const
	{
		return GetTransform()->GetForward();
	}

	/**
	 * \brief	��ȡ����λ��
	 */
	const Vector3& Light::GetLightPosition() const
	{
		return GetTransform()->GetPosition();
	}

	/**
	 * \brief �����л�
	 * \param pNode ���ýڵ�ָ��
	 * \return �����л��Ƿ�ɹ�
	 */
	bool Light::Deserialize(const XmlNode* pNode)
	{
		m_LightType = static_cast<ELightType>(XmlGetAttributeValue<uint8_t>(pNode, "type"));
		m_LightColor.Set(
			XmlGetAttributeValue<float>(pNode, "r"),
			XmlGetAttributeValue<float>(pNode, "g"),
			XmlGetAttributeValue<float>(pNode, "b"),
			XmlGetAttributeValue<float>(pNode, "a"));
		return true;
	}
	
	/**
	 * \brief	Awake
	 */
	void Light::Awake()
	{
		switch (m_LightType)
		{
			case ELightType::DirectionalLight:
				LightManager::SetDirectionalLight(this);
			default:
				break;
		}
	}
	
	/**
	 * \brief	OnDestroy
	 */
	void Light::OnDestroy()
	{
		switch (m_LightType)
		{
			case ELightType::DirectionalLight:
				LightManager::SetDirectionalLight(nullptr);
			default:
				break;
		}
	}
}

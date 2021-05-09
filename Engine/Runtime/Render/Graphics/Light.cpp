#include "GameObject/Transform.h"
#include "Render/Graphics/Light.h"
#include "Render/Graphics/LightManager.h"

namespace SaplingEngine
{
	/**
	 * \brief	获取光照方向
	 */
	Vector3 Light::GetLightDirection() const
	{
		return GetTransform()->GetForward();
	}

	/**
	 * \brief	获取光照位置
	 */
	const Vector3& Light::GetLightPosition() const
	{
		return GetTransform()->GetPosition();
	}

	/**
	 * \brief 反序列化
	 * \param pNode 配置节点指针
	 * \return 反序列化是否成功
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

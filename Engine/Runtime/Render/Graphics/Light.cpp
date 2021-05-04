#include "Light.h"

namespace SaplingEngine
{
	Light* Light::m_pInstance = nullptr;
	
	Light::Light() : Component(ComponentType_Light)
	{
		m_pInstance = this;
	}

	Light::~Light()
	{
		if (m_pInstance == this)
		{
			m_pInstance = nullptr;
		}
	}

	/**
	 * \brief 反序列化
	 * \param pNode 配置节点指针
	 * \return 反序列化是否成功
	 */
	bool Light::Deserialize(const XmlNode* pNode)
	{
		return true;
	}
}

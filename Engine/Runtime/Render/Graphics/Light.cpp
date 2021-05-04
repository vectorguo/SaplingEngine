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
	 * \brief �����л�
	 * \param pNode ���ýڵ�ָ��
	 * \return �����л��Ƿ�ɹ�
	 */
	bool Light::Deserialize(const XmlNode* pNode)
	{
		return true;
	}
}

#include "Material.h"

namespace SaplingEngine
{
	Material::Material()
	{
		
	}

	Material::~Material()
	{
		
	}

	/**
	 * \brief �����л�
	 * \param pNode ���ýڵ�ָ��
	 * \return �����л��Ƿ�ɹ�
	 */
	bool Material::Deserialize(const XmlNode* pNode)
	{
		m_ShaderName = XmlGetAttributeValue<const char*>(pNode, "shader");
		
		return true;
	}
}

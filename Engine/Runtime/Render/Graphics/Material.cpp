#include "Material.h"
#include "ShaderManager.h"

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
		auto shaderName = XmlGetAttributeValue<const char*>(pNode, "shader");
		m_pShader = ShaderManager::GetShader(shaderName);
		
		return true;
	}
}

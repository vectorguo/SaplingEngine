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
	 * \brief 反序列化
	 * \param pNode 配置节点指针
	 * \return 反序列化是否成功
	 */
	bool Material::Deserialize(const XmlNode* pNode)
	{
		m_ShaderName = XmlGetAttributeValue<const char*>(pNode, "shader");
		
		return true;
	}
}

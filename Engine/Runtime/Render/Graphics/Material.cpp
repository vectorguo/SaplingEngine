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
	 * \brief 反序列化
	 * \param pNode 配置节点指针
	 * \return 反序列化是否成功
	 */
	bool Material::Deserialize(const XmlNode* pNode)
	{
		auto shaderName = XmlGetAttributeValue<const char*>(pNode, "shader");
		m_pShader = ShaderManager::GetShader(shaderName);
		
		return true;
	}
}

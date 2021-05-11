#include "Render/Graphics/Material.h"
#include "Render/Graphics/TextureManager.h"

namespace SaplingEngine
{
	Material::Material() : 
		m_MainTexture(TextureManager::White)
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
		
		for (const auto* pPropertyNode = pNode->first_node(); pPropertyNode; pPropertyNode = pPropertyNode->next_sibling())
		{
			std::string propertyName = XmlGetAttributeValue<const char*>(pPropertyNode, "name");
			auto propertyType = XmlGetAttributeValue<int32_t>(pPropertyNode, "type");
			switch (propertyType)
			{
				case 0:		//int
					SetInt(propertyName, XmlGetAttributeValue<int32_t>(pPropertyNode, "value"));
					break;
				case 1:		//float
					SetFloat(propertyName, XmlGetAttributeValue<float>(pPropertyNode, "value"));
					break;
				case 2:		//Color
					SetColor(propertyName,
						XmlGetAttributeValue<float>(pPropertyNode, "r"),
						XmlGetAttributeValue<float>(pPropertyNode, "g"),
						XmlGetAttributeValue<float>(pPropertyNode, "b"),
						XmlGetAttributeValue<float>(pPropertyNode, "a"));
					break;
				case 3:		//Vector3
					SetVector3(propertyName,
						XmlGetAttributeValue<float>(pPropertyNode, "x"),
						XmlGetAttributeValue<float>(pPropertyNode, "y"),
						XmlGetAttributeValue<float>(pPropertyNode, "z"));
					break;
				case 4:		//Vector4
					SetVector4(propertyName,
						XmlGetAttributeValue<float>(pPropertyNode, "x"),
						XmlGetAttributeValue<float>(pPropertyNode, "y"),
						XmlGetAttributeValue<float>(pPropertyNode, "z"),
						XmlGetAttributeValue<float>(pPropertyNode, "w"));
					break;
				case 5:		//Texture2D
					if (propertyName == "baseMap")
					{
						m_MainTexture = TextureManager::CreateTexture2D(XmlGetAttributeValue<const char*>(pPropertyNode, "path"));
					}
					break;
				default:
					break;
			}
		}

		return true;
	}
}

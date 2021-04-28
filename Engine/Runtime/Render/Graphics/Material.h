#pragma once

#include "MaterialProperty.h"
#include "Shader.h"

namespace SaplingEngine
{
	class Material final
	{
	public:
		Material();
		~Material();

		Material(const Material&) = delete;
		Material(Material&&) = delete;
		Material& operator=(const Material&) = delete;
		Material& operator=(Material&&) = delete;

		/**
		 * \brief	获取材质使用的Shader的名称
		 * \return	Shader名称
		 */
		const std::string& GetShaderName() const
		{
			return m_pShader->GetName();
		}

		/**
		 * \brief	获取材质使用的Shader的HashValue
		 * \return	Shader的HashValue
		 */
		const size_t GetShaderHashValue() const
		{
			return m_pShader->GetHashValue();
		}
		
		/**
		 * \brief	反序列化
		 * \param	pNode		配置节点指针
		 * \return	反序列化是否成功
		 */
		bool Deserialize(const XmlNode* pNode);


		
	private:
		/**
		 * \brief	材质使用的shader
		 */
		const Shader* m_pShader = nullptr;

		/**
		 * \brief	材质的属性列表
		 */
		std::map<size_t, MaterialProperty> m_Properties;
	};
}
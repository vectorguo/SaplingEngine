#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	//前置声明
	class Shader;
	
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
		 * \brief 获取材质使用的Shader的名称
		 * \return Shader名称
		 */
		const std::string& GetShaderName() const
		{
			return m_ShaderName;
		}
		
		/**
		 * \brief 反序列化
		 * \param pNode 配置节点指针
		 * \return 反序列化是否成功
		 */
		bool Deserialize(const XmlNode* pNode);
		
	private:
		/**
		 * \brief 该材质使用的Shader名称
		 */
		std::string m_ShaderName;
	};
}
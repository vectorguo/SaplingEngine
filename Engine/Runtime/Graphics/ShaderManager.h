#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Shader;
	
	class ShaderManager final
	{
		using ShaderMap = std::map<std::string, Shader*>;
		
	public:
		ShaderManager() = default;
		~ShaderManager() = default;
		
		SINGLETON(ShaderManager)
		
		/**
		 * \brief 初始化
		 */
		void Initialize();

		/**
		 * \brief 销毁
		 */
		void Destroy();

		/**
		 * \brief 获取Shader
		 * \param shaderName Shader名称
		 * \return Shader
		 */
		const Shader* GetShader(const std::string& shaderName) const;

		/**
		 * \brief 获取所有shader
		 * \return 所有shader
		 */
		const ShaderMap& GetAllShaders() const
		{
			return m_Shaders;
		}

	private:
		ShaderMap m_Shaders;
	};
}
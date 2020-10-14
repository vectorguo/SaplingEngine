#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Shader;
	
	class ShaderManager final
	{
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

	private:
		std::unordered_map<std::string, Shader*> m_Shaders;
	};
}
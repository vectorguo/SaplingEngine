#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Shader;
	
	class ShaderManager final
	{
		using ShaderMap = std::map<size_t, Shader*>;
		
	public:
		/**
		 * \brief	初始化
		 */
		static void Initialize();

		/**
		 * \brief	销毁
		 */
		static void Destroy();

		/**
		 * \brief	获取Shader
		 * \param	shaderName	Shader名称
		 * \return	Shader
		 */
		static const Shader* GetShader(const std::string& shaderName);

		/**
		 * \brief	获取Shader
		 * \param	shaderID	ShaderID
		 * \return	Shader
		 */
		static const Shader* GetShader(size_t shaderID);

		/**
		 * \brief	获取所有shader
		 * \return	所有shader
		 */
		static const ShaderMap& GetAllShaders()
		{
			return shaders;
		}

	private:
		static ShaderMap shaders;
	};
}
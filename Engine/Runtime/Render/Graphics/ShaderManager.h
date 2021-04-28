#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Shader;
	
	class ShaderManager final
	{
		using ShaderMap = std::map<std::string, Shader*>;
		
	public:
		/**
		 * \brief ��ʼ��
		 */
		static void Initialize();

		/**
		 * \brief ����
		 */
		static void Destroy();

		/**
		 * \brief ��ȡShader
		 * \param shaderName Shader����
		 * \return Shader
		 */
		static const Shader* GetShader(const std::string& shaderName);

		/**
		 * \brief ��ȡ����shader
		 * \return ����shader
		 */
		static const ShaderMap& GetAllShaders()
		{
			return shaders;
		}

	private:
		static ShaderMap shaders;
	};
}
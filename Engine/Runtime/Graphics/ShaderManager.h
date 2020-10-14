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
		 * \brief ��ʼ��
		 */
		void Initialize();

		/**
		 * \brief ����
		 */
		void Destroy();

		/**
		 * \brief ��ȡShader
		 * \param shaderName Shader����
		 * \return Shader
		 */
		const Shader* GetShader(const std::string& shaderName) const;

	private:
		std::unordered_map<std::string, Shader*> m_Shaders;
	};
}
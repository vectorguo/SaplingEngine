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

		/**
		 * \brief ��ȡ����shader
		 * \return ����shader
		 */
		const ShaderMap& GetAllShaders() const
		{
			return m_Shaders;
		}

	private:
		ShaderMap m_Shaders;
	};
}
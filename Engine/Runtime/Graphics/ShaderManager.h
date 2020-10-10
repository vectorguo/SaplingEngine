#pragma once

#include "Shader.h"
#include "SaplingEnginePch.h"
#include "Utility/Singleton.h"

namespace SaplingEngine
{
	class ShaderManager final : public Singleton<ShaderManager>
	{
	public:
		ShaderManager() = default;
		
		ShaderManager(const ShaderManager&) = delete;
		ShaderManager(ShaderManager&&) = delete;
		ShaderManager& operator= (const ShaderManager&) = delete;
		ShaderManager& operator= (ShaderManager&&) = delete;
		
		~ShaderManager()
		{
			Destroy();
		}
		
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
		const Shader* GetShader(const string& shaderName) const
		{
			const auto iter = m_Shaders.find(shaderName);
			return iter == m_Shaders.end() ? nullptr : iter->second;
		}
		
	private:
		std::unordered_map<std::string, Shader*> m_Shaders;
	};
}
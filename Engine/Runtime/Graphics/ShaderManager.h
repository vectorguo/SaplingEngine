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
		const Shader* GetShader(const string& shaderName) const
		{
			const auto iter = m_Shaders.find(shaderName);
			return iter == m_Shaders.end() ? nullptr : iter->second;
		}
		
	private:
		std::unordered_map<std::string, Shader*> m_Shaders;
	};
}
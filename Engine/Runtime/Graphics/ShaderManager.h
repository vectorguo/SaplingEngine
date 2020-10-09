#pragma once

#include "Shader.h"
#include "SaplingEnginePch.h"
#include "Utility/Singleton.h"

namespace SaplingEngine
{
	class ShaderManager final : public Singleton<ShaderManager>
	{
	public:

	private:
		std::unordered_map<std::string, Shader> m_Shaders;
	};
}
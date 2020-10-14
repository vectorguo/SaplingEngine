#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Scene;
	
	class SceneManager
	{
	public:
		SceneManager() = default;
		~SceneManager() = default;

		SINGLETON(SceneManager)
	};
}

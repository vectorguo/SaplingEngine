#pragma once

#include "SaplingEngineHeader.h"

namespace SaplingEngine
{
	class GameObject
	{
	public:
		GameObject();
		explicit GameObject(const std::string& name);
		explicit GameObject(std::string&& name);
		~GameObject() = default;

		/*
		 * ½ûÖ¹¿½±´ºÍÒÆ¶¯
		 */
		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) = delete;
		GameObject& operator= (const GameObject&) = delete;
		GameObject& operator= (GameObject&&) = delete;

	private:
		
	};
}
#pragma once

#include "SaplingEngineHeader.h"

namespace SaplingEngine
{
	class Component
	{
	public:
		Component();
		virtual ~Component();

		/*
		 * ½ûÖ¹¿½±´ºÍÒÆ¶¯
		 */
		Component(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator= (const Component&) = delete;
		Component& operator= (Component&&) = delete;

	private:
	};
}
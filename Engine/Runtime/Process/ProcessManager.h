#pragma once

#include <Utility/Singleton.h>

namespace SaplingEngine
{
	class ProcessManager
	{
	public:
		ProcessManager() = default;
		~ProcessManager() = default;

		SINGLETON(ProcessManager)
	};
}
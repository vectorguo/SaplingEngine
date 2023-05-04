#pragma once

#include <Runtime/Utility/Singleton.h>

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
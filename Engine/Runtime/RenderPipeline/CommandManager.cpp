#include "CommandManager.h"

namespace SaplingEngine
{
	CommandManager* CommandManager::m_Instance = nullptr;
	
	CommandManager::CommandManager()
	{
		m_Instance = this;
	}
}

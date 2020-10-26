#include "GraphicsManager.h"

namespace SaplingEngine
{
	GraphicsManager* GraphicsManager::m_Instance = nullptr;
	
	GraphicsManager::GraphicsManager()
	{
		m_Instance = this;
	}
}

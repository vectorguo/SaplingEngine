#pragma once

#include "SaplingEngineHeader.h"

namespace Warcraft
{
	//组件类型
	constexpr uint32_t ComponentType_WarcraftGameMain	= SaplingEngine::ComponentType_InternalLimit + 1;
	constexpr uint32_t ComponentType_WarcraftInput		= SaplingEngine::ComponentType_InternalLimit + 2;

	constexpr uint32_t ComponentType_Character			= SaplingEngine::ComponentType_InternalLimit + 200;
}
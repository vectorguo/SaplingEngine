#pragma once

#include "SaplingEngineHeader.h"

#define SaplingEngineExport _declspec(dllexport)

extern "C"
{
	SaplingEngineExport int Test();
}
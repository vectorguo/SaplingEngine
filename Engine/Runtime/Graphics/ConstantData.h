#pragma once

#include "Math/Matrix4x4.h"

namespace SaplingEngine
{
	using namespace Math;
	
	struct ObjectConstantData
	{
		Matrix4x4 ModelViewProj = Matrix4x4::Identity;
	};

	struct PassConstantData
	{
		
	};
}

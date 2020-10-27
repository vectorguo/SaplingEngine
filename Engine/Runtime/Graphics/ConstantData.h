#pragma once

#include "Math/Matrix4x4.h"

namespace SaplingEngine
{
	using namespace Math;
	
	struct ObjectConstantData
	{
		Matrix4x4 ModelToWorldMatrix = Matrix4x4::Identity;
		Matrix4x4 WorldToProjectionMatrix = Matrix4x4::Identity;
	};

	struct PassConstantData
	{
		
	};
}

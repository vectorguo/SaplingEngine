#pragma once

#include "Math/Matrix4x4.h"

namespace SaplingEngine
{
	using namespace Math;
	
	struct ObjectConstantData
	{
		Matrix4x4 SAPLING_MATRIX_M = Matrix4x4::Identity;
	};

	struct PassConstantData
	{
		Matrix4x4 SAPLING_MATRIX_V = Matrix4x4::Identity;
		Matrix4x4 SAPLING_MATRIX_VP = Matrix4x4::Identity;

		XMFLOAT3 MAIN_LIGHT_POSITION = {0, 0, 0};
		XMFLOAT3 MAIN_LIGHT_DIRECTION = { 0, 0, 0 };
	};
}

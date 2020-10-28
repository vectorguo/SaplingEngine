#pragma once

#include "Color.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

namespace SaplingEngine
{
	namespace Math
	{
		/**
		 * \brief 角度转弧度
		 * \param angle 角度
		 * \return 弧度
		 */
		inline float AngleToRadian(float angle)
		{
			return angle * 0.0174533f;
		}

		/**
		 * \brief 弧度转角度
		 * \param radian 弧度
		 * \return 角度
		 */
		inline float RadianToAngle(float radian)
		{
			return radian * 57.2957795f;
		}
	}
}
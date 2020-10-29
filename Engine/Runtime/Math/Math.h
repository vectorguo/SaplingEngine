#pragma once

namespace SaplingEngine
{
	namespace Math
	{
		/**
		 * \brief �Ƕ�ת����
		 * \param angle �Ƕ�
		 * \return ����
		 */
		inline float AngleToRadian(float angle)
		{
			return angle * 0.0174533f;
		}

		/**
		 * \brief ����ת�Ƕ�
		 * \param radian ����
		 * \return �Ƕ�
		 */
		inline float RadianToAngle(float radian)
		{
			return radian * 57.2957795f;
		}
	}
}
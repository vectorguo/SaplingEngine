#pragma once

#include "Vector3.h"

namespace SaplingEngine
{
	namespace Math
	{
		class Quaternion
		{
		public:
			Quaternion(): m_Value()
			{
				
			}

			explicit Quaternion(FXMVECTOR v): m_Value(v)
			{
				
			}

			/*
			 * operator overloading
			 */
			Quaternion operator* (const Quaternion& q) const
			{
				return Quaternion(XMQuaternionMultiply(m_Value, q.m_Value));
			}

			Quaternion& operator*= (const Quaternion& q)
			{
				m_Value = XMQuaternionMultiply(m_Value, q.m_Value);
				return *this;
			}

			Vector3 operator* (const Vector3& v) const
			{
				return Vector3(XMVector3TransformNormal(v, XMMatrixRotationQuaternion(m_Value)));
			}

			operator XMVECTOR() const
			{
				return m_Value;
			}

			void Normalize()
			{
				m_Value = XMQuaternionNormalize(m_Value);
			}
			
			/*
			 * Creates a rotation which rotates angle degrees around axis
			 */
			static Quaternion RotateAxis(const Vector3& axis, const float angle)
			{
				return Quaternion(XMQuaternionRotationAxis(axis, angle));
			}

			static Quaternion RotateRollPitchYaw(const float pitch, const float yaw, const float roll)
			{
				return Quaternion(XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
			}

			/*
			 * Spherically interpolates between a and b by t. The parameter t is clamped to the range [0, 1]
			 */
			static Quaternion Slerp(const Quaternion& a, const Quaternion& b, const float t)
			{
				return Quaternion(XMQuaternionSlerp(a.m_Value, b.m_Value, t));
			}

		public:
			const static Quaternion Identity;
			
		private:
			XMVECTOR m_Value;
		};
	}
}
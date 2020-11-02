#pragma once

#include "Math.h"
#include "Vector3.h"

namespace SaplingEngine
{
	namespace Math
	{
		class Quaternion
		{
		public:
			Quaternion(): value()
			{
				
			}

			Quaternion(const float x, const float y, const float z, const float w) : value(x, y, z, w)
			{

			}

			explicit Quaternion(FXMVECTOR v): value()
			{
				XMStoreFloat4(&value, v);
			}

			/*
			 * operator overloading
			 */
			Quaternion operator* (const Quaternion& q) const
			{
				return Quaternion(XMQuaternionMultiply(XMLoadFloat4(&value), XMLoadFloat4(&q.value)));
			}

			Quaternion& operator*= (const Quaternion& q)
			{
				XMStoreFloat4(&value, XMQuaternionMultiply(XMLoadFloat4(&value), XMLoadFloat4(&q.value)));
				return *this;
			}

			Vector3 operator* (const Vector3& v) const
			{
				return Vector3(XMVector3TransformNormal(v, XMMatrixRotationQuaternion(XMLoadFloat4(&value))));
			}

			operator XMVECTOR() const
			{
				return XMLoadFloat4(&value);
			}

			void Set(const float x, const float y, const float z, const float w)
			{
				this->x = x;
				this->y = y;
				this->z = z;
				this->w = w;
			}

			void Normalize()
			{
				XMStoreFloat4(&value, XMQuaternionNormalize(XMLoadFloat4(&value)));
			}
			
			/*
			 * Creates a rotation which rotates angle degrees around axis
			 */
			static Quaternion RotateAxis(const Vector3& axis, const float angle)
			{
				return Quaternion(XMQuaternionRotationAxis(axis, AngleToRadian(angle)));
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
				return Quaternion(XMQuaternionSlerp(XMLoadFloat4(&a.value), XMLoadFloat4(&b.value), t));
			}

		public:
			const static Quaternion Identity;
			
		public:
			union
			{
				XMFLOAT4 value;

				struct
				{
					float x;
					float y;
					float z;
					float w;
				};
			};
		};
	}
}
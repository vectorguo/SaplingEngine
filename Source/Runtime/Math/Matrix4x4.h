#pragma once

#include "Quaternion.h"
#include "Vector3.h"
#include "Vector4.h"

namespace SaplingEngine
{
	namespace Math
	{
		class Matrix4x4
		{
		public:
			Matrix4x4(): m_Value()
			{
				
			}

			Matrix4x4(const float m00, const float m01, const float m02, const float m03,
					  const float m10, const float m11, const float m12, const float m13, 
					  const float m20, const float m21, const float m22, const float m23, 
					  const float m30, const float m31, const float m32, const float m33):
				m_Value(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33)
			{
				
			}

			explicit Matrix4x4(FXMMATRIX m): m_Value()
			{
				XMStoreFloat4x4(&m_Value, m);
			}

			/*
			 * operator overloading
			 */
			Matrix4x4 operator* (const Matrix4x4& m) const
			{
				return Matrix4x4(XMMatrixMultiply(Value(), m.Value()));
			}

			Matrix4x4& operator*= (const Matrix4x4& m)
			{
				XMStoreFloat4x4(&m_Value, XMMatrixMultiply(Value(), m.Value()));
				return *this;
			}
			
			/*
			 * convert to matrix
			*/
			XMMATRIX Value() const
			{
				return XMLoadFloat4x4(&m_Value);
			}

			/*
			 * inverse
			 */
			Matrix4x4 Inverse() const
			{
				const auto matrix = Value();
				auto determinant = XMMatrixDeterminant(matrix);
				return Matrix4x4(XMMatrixInverse(&determinant, matrix));
			}
			
			/*
			 * transpose
			 */
			Matrix4x4 Transpose() const
			{
				return Matrix4x4(XMMatrixTranspose(Value()));
			}

			/*
			 * multiply
			 */
			Vector3 MultiplyPoint(const Vector3& v) const
			{
				return Vector3(XMVector3TransformCoord(v.Value(), Value()));
			}
			
			Vector3 MultiplyVector(const Vector3& v) const
			{
				return Vector3(XMVector3TransformNormal(v.Value(), Value()));
			}

			/*
			 * Create an orthogonal projection matrix
			 */
			static Matrix4x4 Orthographic(const float viewWidth, const float viewHeight, const float nearZ, const float farZ)
			{
				return Matrix4x4(XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ));
			}

			/*
			 * Create a perspective projection matrix
			 */
			static Matrix4x4 Perspective(const float fovAngleY, const float aspectRatio, const float nearZ, const float farZ)
			{
				return Matrix4x4(XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ));
			}
			
			/*
			 * Create a "look at" matrix
			 */
			static Matrix4x4 LookAt(const Vector3& eyePosition, const Vector3& focusPosition, const Vector3& upDirection)
			{
				return Matrix4x4(XMMatrixLookAtLH(eyePosition.Value(), focusPosition.Value(), upDirection.Value()));
			}

			/*
			 * Create a "look to" matrix
			 */
			static Matrix4x4 LookTo(const Vector3& eyePosition, const Vector3& eyeDirection, const Vector3& upDirection)
			{
				return Matrix4x4(XMMatrixLookToLH(eyePosition.Value(), eyeDirection.Value(), upDirection.Value()));
			}
			
			/*
			 * Creates a rotation matrix
			 */
			static Matrix4x4 RotateX(const float angle)
			{
				return Matrix4x4(XMMatrixRotationX(angle));
			}

			static Matrix4x4 RotateY(const float angle)
			{
				return Matrix4x4(XMMatrixRotationY(angle));
			}

			static Matrix4x4 RotateZ(const float angle)
			{
				return Matrix4x4(XMMatrixRotationZ(angle));
			}

			static Matrix4x4 Rotate(const Vector3& axis, const float angle)
			{
				return Matrix4x4(XMMatrixRotationAxis(axis.Value(), angle));
			}

			static Matrix4x4 Rotate(const Quaternion& q)
			{
				return Matrix4x4(XMMatrixRotationQuaternion(q.Value()));
			}

			static Matrix4x4 RotateRollPitchYaw(const float pitch, const float yaw, const float roll)
			{
				return Matrix4x4(XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
			}

			static Matrix4x4 RotateRollPitchYaw(const Vector3& v)
			{
				return Matrix4x4(XMMatrixRotationRollPitchYawFromVector(v.Value()));
			}
			
			/*
			 * Create a scaling matrix
			 */
			static Matrix4x4 Scale(const float x, const float y, const float z)
			{
				return Matrix4x4(XMMatrixScaling(x, y, z));
			}
			
			static Matrix4x4 Scale(const Vector3& scale)
			{
				return Matrix4x4(XMMatrixScalingFromVector(scale.Value()));
			}

			/*
			 * Creates a translation matrix
			 */
			static Matrix4x4 Translate(const float x, const float y, const float z)
			{
				return Matrix4x4(XMMatrixTranslation(x, y, z));
			}

			static Matrix4x4 Translate(const Vector3& v)
			{
				return Matrix4x4(XMMatrixTranslationFromVector(v.Value()));
			}
			
		public:
			const static Matrix4x4 Identity;
			
		private:
			XMFLOAT4X4 m_Value;
		};
	}
}
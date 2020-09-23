#pragma once

#include "Application/DirectX12/D3D12Header.h"

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
				return Matrix4x4(XMMatrixMultiply(ToMatrix(), m.ToMatrix()));
			}

			Matrix4x4& operator*= (const Matrix4x4& m)
			{
				XMStoreFloat4x4(&m_Value, XMMatrixMultiply(ToMatrix(), m.ToMatrix()));
				return *this;
			}
			
			/*
			 * convert to matrix
			*/
			XMMATRIX ToMatrix() const
			{
				return XMLoadFloat4x4(&m_Value);
			}

			/*
			 * Äæ¾ØÕó
			 */
			Matrix4x4 Inverse() const
			{
				const auto matrix = ToMatrix();
				auto determinant = XMMatrixDeterminant(matrix);
				return Matrix4x4(XMMatrixInverse(&determinant, matrix));
			}
			
			/*
			 * ×ªÖÃ¾ØÕó
			 */
			Matrix4x4 Transpose() const
			{
				return Matrix4x4(XMMatrixTranspose(ToMatrix()));
			}

		public:
			const static Matrix4x4 Identity;
			
		private:
			XMFLOAT4X4 m_Value;
		};
	}
}
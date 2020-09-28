#pragma once

#include "Application/DirectX12/D3D12Header.h"

namespace SaplingEngine
{
	namespace Math
	{
		class Vector3
		{
		public:
			Vector3() : m_Value()
			{
				
			}

			Vector3(const float x, const float y, const float z): m_Value(x, y, z)
			{
				
			}
			
			explicit Vector3(FXMVECTOR v): m_Value()
			{
				XMStoreFloat3(&m_Value, v);
			}

			/*
			 * operator overloading
			 */
			Vector3 operator+ (const Vector3& v) const
			{
				return Vector3(XMLoadFloat3(&m_Value) + XMLoadFloat3(&v.m_Value));
			}

			Vector3 operator- (const Vector3& v) const
			{
				return Vector3(XMLoadFloat3(&m_Value) - XMLoadFloat3(&v.m_Value));
			}

			Vector3 operator* (const Vector3& v) const
			{
				return Vector3(XMLoadFloat3(&m_Value) * XMLoadFloat3(&v.m_Value));
			}

			Vector3 operator/ (const Vector3& v) const
			{
				return Vector3(XMLoadFloat3(&m_Value) / XMLoadFloat3(&v.m_Value));
			}

			Vector3 operator* (const float s) const
			{
				return Vector3(XMLoadFloat3(&m_Value) * s);
			}

			Vector3 operator/ (const float s) const
			{
				return Vector3(XMLoadFloat3(&m_Value) / s);
			}

			Vector3& operator+= (const Vector3& v)
			{
				Vector3(XMLoadFloat3(&m_Value) + XMLoadFloat3(&v.m_Value));
				return *this;
			}

			Vector3& operator-= (const Vector3& v)
			{
				XMStoreFloat3(&m_Value, XMLoadFloat3(&m_Value) - XMLoadFloat3(&v.m_Value));
				return *this;
			}

			Vector3& operator*= (const Vector3& v)
			{
				XMStoreFloat3(&m_Value, XMLoadFloat3(&m_Value) * XMLoadFloat3(&v.m_Value));
				return *this;
			}

			Vector3& operator/= (const Vector3& v)
			{
				XMStoreFloat3(&m_Value, XMLoadFloat3(&m_Value) / XMLoadFloat3(&v.m_Value));
				return *this;
			}

			Vector3& operator*= (const float s)
			{
				XMStoreFloat3(&m_Value, XMLoadFloat3(&m_Value) * s);
				return *this;
			}

			Vector3& operator/= (const float s)
			{
				XMStoreFloat3(&m_Value, XMLoadFloat3(&m_Value) / s);
				return *this;
			}

			bool operator== (const Vector3& v) const
			{
				return XMVector3Equal(Value(), v.Value());
			}

			bool operator!= (const Vector3& v) const
			{
				return XMVector3NotEqual(Value(), v.Value());
			}
			
			/*
			 * convert to vector
			 */
			XMVECTOR Value() const
			{
				return XMLoadFloat3(&m_Value);
			}

			float Length() const
			{
				return XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_Value)));
			}

			float LengthSq() const
			{
				return XMVectorGetX(XMVector3LengthSq(XMLoadFloat3(&m_Value)));
			}
			
			Vector3& Normalize()
			{
				XMVector3Normalize(XMLoadFloat3(&m_Value));
				return *this;
			}
			
			float Dot(const Vector3* v) const
			{
				return XMVectorGetX(XMVector3Dot(XMLoadFloat3(&m_Value), XMLoadFloat3(&v->m_Value)));
			}

			float Dot(const Vector3& v) const
			{
				return XMVectorGetX(XMVector3Dot(XMLoadFloat3(&m_Value), XMLoadFloat3(&v.m_Value)));
			}
			
			Vector3 Cross(const Vector3* v) const
			{
				return Vector3(XMVector3Cross(XMLoadFloat3(&m_Value), XMLoadFloat3(&v->m_Value)));
			}

			Vector3 Cross(const Vector3& v) const
			{
				return Vector3(XMVector3Cross(XMLoadFloat3(&m_Value), XMLoadFloat3(&v.m_Value)));
			}

		public:
			const static Vector3 Zero;
			const static Vector3 One;
			const static Vector3 Forward;
			const static Vector3 Right;
			const static Vector3 Up;
			
		private:
			XMFLOAT3 m_Value;
		};
	}
}

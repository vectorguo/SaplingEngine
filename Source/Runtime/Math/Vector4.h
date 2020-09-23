#pragma once

#include "Application/DirectX12/D3D12Header.h"

namespace SaplingEngine
{
	namespace Math
	{
		class Vector4
		{
			Vector4(): m_Value()
			{

			}

			Vector4(const float x, const float y, const float z, const float w) : m_Value(x, y, z, w)
			{

			}

			explicit Vector4(FXMVECTOR v) : m_Value()
			{
				XMStoreFloat4(&m_Value, v);
			}

			/*
			 * operator overloading
			 */
			Vector4 operator+ (const Vector4& v) const
			{
				return Vector4(XMLoadFloat4(&m_Value) + XMLoadFloat4(&v.m_Value));
			}

			Vector4 operator- (const Vector4& v) const
			{
				return Vector4(XMLoadFloat4(&m_Value) - XMLoadFloat4(&v.m_Value));
			}

			Vector4 operator* (const Vector4& v) const
			{
				return Vector4(XMLoadFloat4(&m_Value) * XMLoadFloat4(&v.m_Value));
			}

			Vector4 operator/ (const Vector4& v) const
			{
				return Vector4(XMLoadFloat4(&m_Value) / XMLoadFloat4(&v.m_Value));
			}

			Vector4 operator* (const float s) const
			{
				return Vector4(XMVectorScale(XMLoadFloat4(&m_Value), s));
			}

			Vector4 operator/ (const float s) const
			{
				return Vector4(XMLoadFloat4(&m_Value) / s);
			}

			Vector4& operator+= (const Vector4& v)
			{
				Vector4(XMLoadFloat4(&m_Value) + XMLoadFloat4(&v.m_Value));
				return *this;
			}

			Vector4& operator-= (const Vector4& v)
			{
				XMStoreFloat4(&m_Value, XMLoadFloat4(&m_Value) - XMLoadFloat4(&v.m_Value));
				return *this;
			}

			Vector4& operator*= (const Vector4& v)
			{
				XMStoreFloat4(&m_Value, XMLoadFloat4(&m_Value) * XMLoadFloat4(&v.m_Value));
				return *this;
			}

			Vector4& operator/= (const Vector4& v)
			{
				XMStoreFloat4(&m_Value, XMLoadFloat4(&m_Value) / XMLoadFloat4(&v.m_Value));
				return *this;
			}

			Vector4& operator*= (const float s)
			{
				XMStoreFloat4(&m_Value, XMVectorScale(XMLoadFloat4(&m_Value), s));
				return *this;
			}

			Vector4& operator/= (const float s)
			{
				XMStoreFloat4(&m_Value, XMLoadFloat4(&m_Value) / s);
				return *this;
			}

			/*
			 * convert to vector
			 */
			XMVECTOR ToVector() const
			{
				return XMLoadFloat4(&m_Value);
			}

			float Length() const
			{
				return XMVectorGetX(XMVector4Length(XMLoadFloat4(&m_Value)));
			}

			float LengthSq() const
			{
				return XMVectorGetX(XMVector4LengthSq(XMLoadFloat4(&m_Value)));
			}

			Vector4& Normalize()
			{
				XMVector4Normalize(XMLoadFloat4(&m_Value));
				return *this;
			}

		public:
			const static Vector4 Zero;
			const static Vector4 One;
			
		private:
			XMFLOAT4 m_Value;
		};
	}
}

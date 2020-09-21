#pragma once

#include "Application/DirectX12/D3D12Header.h"

namespace SaplingEngine
{
	namespace Math
	{
		struct Vector4 : XMFLOAT4
		{
			Vector4() : XMFLOAT4()
			{

			}

			Vector4(const float x, const float y, const float z, const float w) : XMFLOAT4(x, y, z, w)
			{

			}

			explicit Vector4(FXMVECTOR v) : XMFLOAT4()
			{
				XMStoreFloat4(this, v);
			}

			/*
			 * operator overloading
			 */
			Vector4 operator+ (const Vector4& v) const
			{
				return Vector4(XMLoadFloat4(this) + XMLoadFloat4(&v));
			}

			Vector4 operator- (const Vector4& v) const
			{
				return Vector4(XMLoadFloat4(this) - XMLoadFloat4(&v));
			}

			Vector4 operator* (const Vector4& v) const
			{
				return Vector4(XMLoadFloat4(this) * XMLoadFloat4(&v));
			}

			Vector4 operator/ (const Vector4& v) const
			{
				return Vector4(XMLoadFloat4(this) / XMLoadFloat4(&v));
			}

			Vector4 operator* (const float s) const
			{
				return Vector4(XMVectorScale(XMLoadFloat4(this), s));
			}

			Vector4 operator/ (const float s) const
			{
				return Vector4(XMLoadFloat4(this) / s);
			}

			Vector4& operator+= (const Vector4& v)
			{
				Vector4(XMLoadFloat4(this) + XMLoadFloat4(&v));
				return *this;
			}

			Vector4& operator-= (const Vector4& v)
			{
				XMStoreFloat4(this, XMLoadFloat4(this) - XMLoadFloat4(&v));
				return *this;
			}

			Vector4& operator*= (const Vector4& v)
			{
				XMStoreFloat4(this, XMLoadFloat4(this) * XMLoadFloat4(&v));
				return *this;
			}

			Vector4& operator/= (const Vector4& v)
			{
				XMStoreFloat4(this, XMLoadFloat4(this) / XMLoadFloat4(&v));
				return *this;
			}

			Vector4& operator*= (const float s)
			{
				XMStoreFloat4(this, XMVectorScale(XMLoadFloat4(this), s));
				return *this;
			}

			Vector4& operator/= (const float s)
			{
				XMStoreFloat4(this, XMLoadFloat4(this) / s);
				return *this;
			}

			/*
			 * convert to vector
			 */
			XMVECTOR ToVector() const
			{
				return XMLoadFloat4(this);
			}

			float Length() const
			{
				return XMVectorGetX(XMVector4Length(XMLoadFloat4(this)));
			}

			float LengthSq() const
			{
				return XMVectorGetX(XMVector4LengthSq(XMLoadFloat4(this)));
			}

			Vector4& Normalize()
			{
				XMVector4Normalize(XMLoadFloat4(this));
				return *this;
			}
		};
	}
}

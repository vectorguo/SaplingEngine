#pragma once

#include "Application/DirectX12/D3D12Header.h"

namespace SaplingEngine
{
	namespace Math
	{
		struct Vector3 : XMFLOAT3
		{
			Vector3() : XMFLOAT3()
			{
				
			}

			Vector3(const float x, const float y, const float z): XMFLOAT3(x, y, z)
			{
				
			}
			
			explicit Vector3(FXMVECTOR v) : XMFLOAT3()
			{
				XMStoreFloat3(this, v);
			}

			/*
			 * operator overloading
			 */
			Vector3 operator+ (const Vector3& v) const
			{
				return Vector3(XMLoadFloat3(this) + XMLoadFloat3(&v));
			}

			Vector3 operator- (const Vector3& v) const
			{
				return Vector3(XMLoadFloat3(this) - XMLoadFloat3(&v));
			}

			Vector3 operator* (const Vector3& v) const
			{
				return Vector3(XMLoadFloat3(this) * XMLoadFloat3(&v));
			}

			Vector3 operator/ (const Vector3& v) const
			{
				return Vector3(XMLoadFloat3(this) / XMLoadFloat3(&v));
			}

			Vector3 operator* (const float s) const
			{
				return Vector3(XMLoadFloat3(this) * s);
			}

			Vector3 operator/ (const float s) const
			{
				return Vector3(XMLoadFloat3(this) / s);
			}

			Vector3& operator+= (const Vector3& v)
			{
				Vector3(XMLoadFloat3(this) + XMLoadFloat3(&v));
				return *this;
			}

			Vector3& operator-= (const Vector3& v)
			{
				XMStoreFloat3(this, XMLoadFloat3(this) - XMLoadFloat3(&v));
				return *this;
			}

			Vector3& operator*= (const Vector3& v)
			{
				XMStoreFloat3(this, XMLoadFloat3(this) * XMLoadFloat3(&v));
				return *this;
			}

			Vector3& operator/= (const Vector3& v)
			{
				XMStoreFloat3(this, XMLoadFloat3(this) / XMLoadFloat3(&v));
				return *this;
			}

			Vector3& operator*= (const float s)
			{
				XMStoreFloat3(this, XMLoadFloat3(this) * s);
				return *this;
			}

			Vector3& operator/= (const float s)
			{
				XMStoreFloat3(this, XMLoadFloat3(this) / s);
				return *this;
			}
			
			/*
			 * convert to vector
			 */
			XMVECTOR ToVector() const
			{
				return XMLoadFloat3(this);
			}

			float Length() const
			{
				return XMVectorGetX(XMVector3Length(XMLoadFloat3(this)));
			}

			float LengthSq() const
			{
				return XMVectorGetX(XMVector3LengthSq(XMLoadFloat3(this)));
			}
			
			Vector3& Normalize()
			{
				XMVector3Normalize(XMLoadFloat3(this));
				return *this;
			}
			
			float Dot(const Vector3* v) const
			{
				return XMVectorGetX(XMVector3Dot(XMLoadFloat3(this), XMLoadFloat3(v)));
			}

			float Dot(const Vector3& v) const
			{
				return XMVectorGetX(XMVector3Dot(XMLoadFloat3(this), XMLoadFloat3(&v)));
			}
			
			Vector3 Cross(const Vector3* v) const
			{
				return Vector3(XMVector3Cross(XMLoadFloat3(this), XMLoadFloat3(v)));
			}

			Vector3 Cross(const Vector3& v) const
			{
				return Vector3(XMVector3Cross(XMLoadFloat3(this), XMLoadFloat3(&v)));
			}
		};
	}
}

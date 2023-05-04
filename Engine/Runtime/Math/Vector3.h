#pragma once

#include "Runtime/Render/Graphics/DirectX12/Dx12Utility.h"

namespace SaplingEngine
{
	namespace Math
	{
		struct Vector3
		{
			Vector3() : value()
			{
				
			}

			Vector3(const float x, const float y, const float z): value(x, y, z)
			{
				
			}

			Vector3(XMFLOAT3 v) : value(v)
			{
			}
			
			explicit Vector3(FXMVECTOR v): value()
			{
				XMStoreFloat3(&value, v);
			}

			/*
			 * operator overloading
			 */
			Vector3 operator+ (const Vector3& v) const
			{
				return Vector3(XMLoadFloat3(&value) + XMLoadFloat3(&v.value));
			}

			Vector3 operator- (const Vector3& v) const
			{
				return Vector3(XMLoadFloat3(&value) - XMLoadFloat3(&v.value));
			}

			Vector3 operator* (const Vector3& v) const
			{
				return Vector3(XMLoadFloat3(&value) * XMLoadFloat3(&v.value));
			}

			Vector3 operator/ (const Vector3& v) const
			{
				return Vector3(XMLoadFloat3(&value) / XMLoadFloat3(&v.value));
			}

			Vector3 operator* (const float s) const
			{
				return Vector3(XMLoadFloat3(&value) * s);
			}

			Vector3 operator/ (const float s) const
			{
				return Vector3(XMLoadFloat3(&value) / s);
			}

			Vector3& operator+= (const Vector3& v)
			{
				XMStoreFloat3(&value, XMLoadFloat3(&value) + XMLoadFloat3(&v.value));
				return *this;
			}

			Vector3& operator-= (const Vector3& v)
			{
				XMStoreFloat3(&value, XMLoadFloat3(&value) - XMLoadFloat3(&v.value));
				return *this;
			}

			Vector3& operator*= (const Vector3& v)
			{
				XMStoreFloat3(&value, XMLoadFloat3(&value) * XMLoadFloat3(&v.value));
				return *this;
			}

			Vector3& operator/= (const Vector3& v)
			{
				XMStoreFloat3(&value, XMLoadFloat3(&value) / XMLoadFloat3(&v.value));
				return *this;
			}

			Vector3& operator*= (const float s)
			{
				XMStoreFloat3(&value, XMLoadFloat3(&value) * s);
				return *this;
			}

			Vector3& operator/= (const float s)
			{
				XMStoreFloat3(&value, XMLoadFloat3(&value) / s);
				return *this;
			}

			bool operator== (const Vector3& v) const
			{
				return XMVector3Equal(XMLoadFloat3(&value), XMLoadFloat3(&v.value));
			}

			bool operator!= (const Vector3& v) const
			{
				return XMVector3NotEqual(XMLoadFloat3(&value), XMLoadFloat3(&v.value));
			}

			operator XMVECTOR() const
			{
				return XMLoadFloat3(&value);
			}

			operator XMFLOAT3() const
			{
				return value;
			}

			void Set(const float x, const float y, const float z)
			{
				this->x = x;
				this->y = y;
				this->z = z;
			}
			
			float Length() const
			{
				return XMVectorGetX(XMVector3Length(XMLoadFloat3(&value)));
			}

			float LengthSq() const
			{
				return XMVectorGetX(XMVector3LengthSq(XMLoadFloat3(&value)));
			}
			
			Vector3& Normalize()
			{
				XMVector3Normalize(XMLoadFloat3(&value));
				return *this;
			}
			
			float Dot(const Vector3* v) const
			{
				return XMVectorGetX(XMVector3Dot(XMLoadFloat3(&value), XMLoadFloat3(&v->value)));
			}

			float Dot(const Vector3& v) const
			{
				return XMVectorGetX(XMVector3Dot(XMLoadFloat3(&value), XMLoadFloat3(&v.value)));
			}
			
			Vector3 Cross(const Vector3* v) const
			{
				return Vector3(XMVector3Cross(XMLoadFloat3(&value), XMLoadFloat3(&v->value)));
			}

			Vector3 Cross(const Vector3& v) const
			{
				return Vector3(XMVector3Cross(XMLoadFloat3(&value), XMLoadFloat3(&v.value)));
			}

			const static Vector3 Zero;
			const static Vector3 One;
			const static Vector3 Forward;
			const static Vector3 Right;
			const static Vector3 Up;
			
			union
			{
				XMFLOAT3 value;

				struct
				{
					float x;
					float y;
					float z;
				};
			};
		};
	}
}

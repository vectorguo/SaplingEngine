#pragma once

#include "RenderPipeline/GraphicsPch.h"

namespace SaplingEngine
{
	namespace Math
	{
		struct Vector4
		{
		public:
			Vector4(): value()
			{

			}

			Vector4(const float x, const float y, const float z, const float w) : value(x, y, z, w)
			{

			}

			explicit Vector4(FXMVECTOR v) : value()
			{
				XMStoreFloat4(&value, v);
			}

			/*
			 * operator overloading
			 */
			Vector4 operator+ (const Vector4& v) const
			{
				return Vector4(XMLoadFloat4(&value) + XMLoadFloat4(&v.value));
			}

			Vector4 operator- (const Vector4& v) const
			{
				return Vector4(XMLoadFloat4(&value) - XMLoadFloat4(&v.value));
			}

			Vector4 operator* (const Vector4& v) const
			{
				return Vector4(XMLoadFloat4(&value) * XMLoadFloat4(&v.value));
			}

			Vector4 operator/ (const Vector4& v) const
			{
				return Vector4(XMLoadFloat4(&value) / XMLoadFloat4(&v.value));
			}

			Vector4 operator* (const float s) const
			{
				return Vector4(XMVectorScale(XMLoadFloat4(&value), s));
			}

			Vector4 operator/ (const float s) const
			{
				return Vector4(XMLoadFloat4(&value) / s);
			}

			Vector4& operator+= (const Vector4& v)
			{
				Vector4(XMLoadFloat4(&value) + XMLoadFloat4(&v.value));
				return *this;
			}

			Vector4& operator-= (const Vector4& v)
			{
				XMStoreFloat4(&value, XMLoadFloat4(&value) - XMLoadFloat4(&v.value));
				return *this;
			}

			Vector4& operator*= (const Vector4& v)
			{
				XMStoreFloat4(&value, XMLoadFloat4(&value) * XMLoadFloat4(&v.value));
				return *this;
			}

			Vector4& operator/= (const Vector4& v)
			{
				XMStoreFloat4(&value, XMLoadFloat4(&value) / XMLoadFloat4(&v.value));
				return *this;
			}

			Vector4& operator*= (const float s)
			{
				XMStoreFloat4(&value, XMVectorScale(XMLoadFloat4(&value), s));
				return *this;
			}

			Vector4& operator/= (const float s)
			{
				XMStoreFloat4(&value, XMLoadFloat4(&value) / s);
				return *this;
			}

			operator XMVECTOR() const
			{
				return XMLoadFloat4(&value);
			}

			float Length() const
			{
				return XMVectorGetX(XMVector4Length(XMLoadFloat4(&value)));
			}

			float LengthSq() const
			{
				return XMVectorGetX(XMVector4LengthSq(XMLoadFloat4(&value)));
			}

			Vector4& Normalize()
			{
				XMVector4Normalize(XMLoadFloat4(&value));
				return *this;
			}

			const static Vector4 Zero;
			const static Vector4 One;
			
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

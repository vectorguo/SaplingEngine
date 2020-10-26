#pragma once

#include "RenderPipeline/GraphicsPch.h"

namespace SaplingEngine
{
	namespace Math
	{
		struct Vector2
		{
			Vector2(): value()
			{

			}

			Vector2(const float x, const float y) : value(x, y)
			{

			}

			explicit Vector2(FXMVECTOR v) : value()
			{
				XMStoreFloat2(&value, v);
			}

			/*
			 * operator overloading
			 */
			Vector2 operator+ (const Vector2& v) const
			{
				return Vector2(XMLoadFloat2(&value) + XMLoadFloat2(&v.value));
			}

			Vector2 operator- (const Vector2& v) const
			{
				return Vector2(XMLoadFloat2(&value) - XMLoadFloat2(&v.value));
			}

			Vector2 operator* (const Vector2& v) const
			{
				return Vector2(XMLoadFloat2(&value) * XMLoadFloat2(&v.value));
			}

			Vector2 operator/ (const Vector2& v) const
			{
				return Vector2(XMLoadFloat2(&value) / XMLoadFloat2(&v.value));
			}

			Vector2 operator* (const float s) const
			{
				return Vector2(XMVectorScale(XMLoadFloat2(&value), s));
			}

			Vector2 operator/ (const float s) const
			{
				return Vector2(XMLoadFloat2(&value) / s);
			}

			Vector2& operator+= (const Vector2& v)
			{
				Vector2(XMLoadFloat2(&value) + XMLoadFloat2(&v.value));
				return *this;
			}

			Vector2& operator-= (const Vector2& v)
			{
				XMStoreFloat2(&value, XMLoadFloat2(&value) - XMLoadFloat2(&v.value));
				return *this;
			}

			Vector2& operator*= (const Vector2& v)
			{
				XMStoreFloat2(&value, XMLoadFloat2(&value) * XMLoadFloat2(&v.value));
				return *this;
			}

			Vector2& operator/= (const Vector2& v)
			{
				XMStoreFloat2(&value, XMLoadFloat2(&value) / XMLoadFloat2(&v.value));
				return *this;
			}

			Vector2& operator*= (const float s)
			{
				XMStoreFloat2(&value, XMVectorScale(XMLoadFloat2(&value), s));
				return *this;
			}

			Vector2& operator/= (const float s)
			{
				XMStoreFloat2(&value, XMLoadFloat2(&value) / s);
				return *this;
			}

			operator XMVECTOR() const
			{
				return XMLoadFloat2(&value);
			}

			float Length() const
			{
				return XMVectorGetX(XMVector2Length(XMLoadFloat2(&value)));
			}

			float LengthSq() const
			{
				return XMVectorGetX(XMVector2LengthSq(XMLoadFloat2(&value)));
			}

			Vector2& Normalize()
			{
				XMVector2Normalize(XMLoadFloat2(&value));
				return *this;
			}

			const static Vector2 Zero;
			const static Vector2 One;
			
			union
			{
				XMFLOAT2 value;

				struct
				{
					float x;
					float y;
				};
			};
		};
	}
}

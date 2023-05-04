#pragma once

#include "Runtime/Render/Graphics/DirectX12/Dx12Utility.h"

namespace SaplingEngine
{
	namespace Math
	{
		struct Color
		{
			Color(): value(0, 0, 0, 1)
			{
				
			}

			Color(const float r, const float g, const float b, const float a): value(r, g, b, a)
			{
				
			}

			Color(const char r, const char g, const char b, const char a): value(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f)
			{
				
			}

			explicit Color(FXMVECTOR v) : value()
			{
				XMStoreFloat4(&value, v);
			}

			/*
			 * operator overloading
			 */
			Color operator+ (const Color& v) const
			{
				return Color(XMLoadFloat4(&value) + XMLoadFloat4(&v.value));
			}

			Color operator- (const Color& v) const
			{
				return Color(XMLoadFloat4(&value) - XMLoadFloat4(&v.value));
			}

			Color operator* (const Color& v) const
			{
				return Color(XMLoadFloat4(&value) * XMLoadFloat4(&v.value));
			}

			Color operator/ (const Color& v) const
			{
				return Color(XMLoadFloat4(&value) / XMLoadFloat4(&v.value));
			}

			Color operator* (const float s) const
			{
				return Color(XMVectorScale(XMLoadFloat4(&value), s));
			}

			Color operator/ (const float s) const
			{
				return Color(XMLoadFloat4(&value) / s);
			}

			Color& operator*= (const float s)
			{
				XMStoreFloat4(&value, XMVectorScale(XMLoadFloat4(&value), s));
				return *this;
			}

			Color& operator/= (const float s)
			{
				XMStoreFloat4(&value, XMLoadFloat4(&value) / s);
				return *this;
			}

			operator const float* () const
			{
				return f;
			}

			void Set(const float r, const float g, const float b, const float a)
			{
				this->r = r;
				this->g = g;
				this->b = b;
				this->a = a;
			}

			const static Color Black;
			const static Color Blue;
			const static Color Cyan;
			const static Color Gold;
			const static Color Gray;
			const static Color Green;
			const static Color LightBlue;
			const static Color Orange;
			const static Color Pink;
			const static Color Red;
			const static Color Silver;
			const static Color White;
			const static Color Yellow;
			
			union
			{
				XMFLOAT4 value;
				
				float f[4];
				
				struct
				{
					float r;
					float g;
					float b;
					float a;
				};
			};
		};
	}
}
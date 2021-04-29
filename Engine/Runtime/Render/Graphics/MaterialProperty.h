#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class MaterialProperty
	{
	public:
		MaterialProperty()
		{
			i = 0;
		}

		~MaterialProperty() = default;

		inline int32_t GetInt() const
		{
			return i;
		}

		inline float GetFloat() const
		{
			return f;
		}

		inline const Color& GetColor() const
		{
			return c;
		}

		inline const Vector4& GetVector4() const
		{
			return v4;
		}

		inline void SetInt(int32_t v)
		{
			i = v;
		}

		inline void SetFloat(float v)
		{
			f = v;
		}

		inline void SetColor(const Color& v)
		{
			c = v;
		}

		inline void SetColor(float r, float g, float b, float a)
		{
			c.r = r;
			c.g = g;
			c.b = b;
			c.a = a;
		}

		inline void SetVector4(const Vector4& v)
		{
			v4 = v;
		}

		inline void SetVector4(float x, float y, float z, float w)
		{
			v4.x = x;
			v4.y = y;
			v4.z = z;
			v4.w = w;
		}

	private:
		union
		{
			int32_t i;
			float	f;
			Color	c;
			Vector4 v4;
		};
	};
}
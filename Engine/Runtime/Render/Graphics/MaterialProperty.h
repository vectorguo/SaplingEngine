#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class MaterialProperty
	{
	public:
		MaterialProperty() = default;
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

		inline const Vector2& GetVector2() const
		{
			return v2;
		}

		inline const Vector3& GetVector3() const
		{
			return v3;
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

		inline void SetVector2(const Vector2& v)
		{
			v2 = v;
		}

		inline void SetVector3(const Vector3& v)
		{
			v3 = v;
		}

		inline void SetVector4(const Vector4& v)
		{
			v4 = v;
		}

	private:
		union
		{
			int32_t i;
			float	f;
			Color	c;
			Vector2 v2;
			Vector3 v3;
			Vector4 v4;
		};
	};
}
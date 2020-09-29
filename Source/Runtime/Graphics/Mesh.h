#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Mesh
	{
	public:
		Mesh();
		
	private:
		std::vector<Vector3> m_Vertices;
		std::vector<Vector3> m_Normals;
		std::vector<Color> m_Colors;
		std::vector<Vector2> m_Uv0;
	};
}
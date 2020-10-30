#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	struct VertexData
	{
		XMFLOAT3 PositionOS;		//Vertex Position in Object Space
		XMFLOAT3 NormalOS;			//Normal in Object Space
		XMFLOAT4 Color;				//Vertex Color
		XMFLOAT2 UV0;				//UV0
		XMFLOAT2 UV1;				//UV1
	};
}

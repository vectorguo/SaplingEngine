#pragma once

#include "RenderPass.h"

namespace SaplingEngine
{
	class RenderOpaquePass final : public RenderPass
	{
	public:
		explicit RenderOpaquePass(const std::string& name)
			: RenderPass(name)
		{
			
		}

		explicit RenderOpaquePass(std::string&& name)
			: RenderPass(name)
		{
			
		}
	};
}
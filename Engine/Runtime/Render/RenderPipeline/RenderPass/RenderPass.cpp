#include "RenderPass.h"

namespace SaplingEngine
{
	RenderPass::RenderPass(const std::string& name) : m_Name(name)
	{
	}

	RenderPass::RenderPass(std::string&& name) : m_Name(std::move(name))
	{
	}

	RenderPass::~RenderPass()
	{
		
	}
}

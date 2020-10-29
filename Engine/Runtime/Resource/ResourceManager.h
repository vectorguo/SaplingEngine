#pragma once

#include "IResource.h"
#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class ResourceManager final
	{
	public:
		ResourceManager() = default;
		~ResourceManager() = default;

		SINGLETON(ResourceManager)


		
	private:
		/**
		 * \brief 加载完的资源列表
		 */
		std::map<std::string, IResourceSptr> m_Resources;
	};
}
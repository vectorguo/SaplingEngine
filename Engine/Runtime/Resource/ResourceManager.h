#pragma once

#include "IResource.h"
#include "MeshResource.h"
#include "ResourceLoader.h"
#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class ResourceManager final
	{
	public:
		ResourceManager() = default;
		~ResourceManager() = default;

		SINGLETON(ResourceManager)

		template <typename T>
		std::shared_ptr<T> LoadResource(const std::string& path);
		
	private:
		/**
		 * \brief 加载完的资源列表
		 */
		std::map<std::string, IResourceSptr> m_Resources;
	};

	template <typename T>
	std::shared_ptr<T> ResourceManager::LoadResource(const std::string& path)
	{
		return nullptr;
	}

	template <>
	inline std::shared_ptr<MeshResource> ResourceManager::LoadResource(const std::string& path)
	{
		const auto iter = m_Resources.find(path);
		if (iter != m_Resources.end())
		{
			return std::static_pointer_cast<MeshResource>(iter->second);
		}
		else
		{
			auto* pMeshResource = LoadMeshResource(path);
			auto meshResourceSptr = std::shared_ptr<MeshResource>(pMeshResource);
			m_Resources.insert_or_assign(path, meshResourceSptr);
			return meshResourceSptr;
		}
	}
}

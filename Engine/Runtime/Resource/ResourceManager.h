#pragma once

#include "IResource.h"
#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class ResourceManager final
	{
		using ResourceMap = std::map<EResourceType, std::map<std::string, IResourceSptr>>;
		using LoadDelegate = std::function<IResource* (const std::string&)>;
		using MeshConfig = std::tuple<std::string, uint32_t, uint32_t>;
		
	public:
		/**
		 * \brief	初始化
		 * \return	初始化是否成功
		 */
		static bool Initialize();

		/**
		 * \brief	获取mesh配置信息
		 * \param	meshName		mesh名称
		 * \return	mesh配置指针
		 */
		static const MeshConfig* GetMeshConfig(const std::string& meshName)
		{
			const auto iter = m_MeshConfigs.find(meshName);
			return iter == m_MeshConfigs.end() ? nullptr : &iter->second;
		}
		
		template <typename T>
		static std::shared_ptr<T> LoadResource(const std::string& path);

	private:
		/**
		 * \brief 加载资源配置
		 */
		static void LoadResourceConfigs();
		
	private:
		/**
		 * \brief 加载完的资源列表
		 */
		static ResourceMap m_Resources;

		/**
		 * \brief 资源加载器
		 */
		static std::map<EResourceType, LoadDelegate> m_ResourceLoaders;

		/**
		 * \brief Mesh资源配置
		 */
		static std::map<std::string, MeshConfig> m_MeshConfigs;
	};

	template <typename T>
	std::shared_ptr<T> ResourceManager::LoadResource(const std::string& path)
	{
		const auto resourceType = T::GetResourceType();
		auto& resources = m_Resources[resourceType];
		const auto iter = resources.find(path);
		if (iter != resources.end())
		{
			return std::static_pointer_cast<T>(iter->second);
		}
		else
		{
			//获取加载器
			auto loaderIter = m_ResourceLoaders.find(resourceType);
			if (loaderIter != m_ResourceLoaders.end())
			{
				auto* pResource = loaderIter->second(path);
				auto resourceSptr = std::shared_ptr<T>(dynamic_cast<T*>(pResource));
				resources.emplace(path, resourceSptr);
				return resourceSptr;
			}
			else
			{
				return nullptr;
			}
		}
	}
}

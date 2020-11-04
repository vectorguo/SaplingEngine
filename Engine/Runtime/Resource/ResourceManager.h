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
		ResourceManager() = default;
		~ResourceManager() = default;

		SINGLETON(ResourceManager)

		/**
		 * \brief ��ʼ��
		 * \return ��ʼ���Ƿ�ɹ�
		 */
		bool Initialize();

		/**
		 * \brief ��ȡmesh������Ϣ
		 * \param meshName mesh����
		 * \return mesh����ָ��
		 */
		const MeshConfig* GetMeshConfig(const std::string& meshName) const
		{
			const auto iter = m_MeshConfigs.find(meshName);
			return iter == m_MeshConfigs.end() ? nullptr : &iter->second;
		}
		
		template <typename T>
		std::shared_ptr<T> LoadResource(const std::string& path);

	private:
		/**
		 * \brief ������Դ����
		 */
		void LoadResourceConfigs();
		
	private:
		/**
		 * \brief ���������Դ�б�
		 */
		ResourceMap m_Resources;

		/**
		 * \brief ��Դ������
		 */
		std::map<EResourceType, LoadDelegate> m_ResourceLoaders;

		/**
		 * \brief Mesh��Դ����
		 */
		std::map<std::string, MeshConfig> m_MeshConfigs;
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
			//��ȡ������
			auto loaderIter = m_ResourceLoaders.find(resourceType);
			if (loaderIter != m_ResourceLoaders.end())
			{
				auto* pResource = loaderIter->second(path);
				auto resourceSptr = std::shared_ptr<T>(dynamic_cast<T*>(pResource));
				resources.insert_or_assign(path, resourceSptr);
				return resourceSptr;
			}
			else
			{
				return nullptr;
			}
		}
	}
}

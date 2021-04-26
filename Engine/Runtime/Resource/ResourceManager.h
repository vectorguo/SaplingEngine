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
		 * \brief	��ʼ��
		 * \return	��ʼ���Ƿ�ɹ�
		 */
		static bool Initialize();

		/**
		 * \brief	��ȡmesh������Ϣ
		 * \param	meshName		mesh����
		 * \return	mesh����ָ��
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
		 * \brief ������Դ����
		 */
		static void LoadResourceConfigs();
		
	private:
		/**
		 * \brief ���������Դ�б�
		 */
		static ResourceMap m_Resources;

		/**
		 * \brief ��Դ������
		 */
		static std::map<EResourceType, LoadDelegate> m_ResourceLoaders;

		/**
		 * \brief Mesh��Դ����
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
			//��ȡ������
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

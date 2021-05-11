#pragma once

#include "Render/Graphics/Mesh.h"
#include "Render/Graphics/Texture2D.h"
#include "Resource/ResourceAsyncRequest.h"
#include "Resource/ResourceLoader.h"

namespace SaplingEngine
{
	class ResourceManager final
	{
	public:
		/**
		 * \brief	初始化
		 * \return	初始化是否成功
		 */
		static bool Initialize();

		/**
		 * \brief	更新
		 */
		static void Update();

		/**
		 * \brief	加载资源
		 * \param	path		资源路径
		 */
		template <typename T>
		static SharedPtr<T> Load(const std::string& path);

		/**
		 * \brief	加载Mesh
		 * \param	path		Mesh路径
		 */
		template <>
		static SharedPtr<Mesh> Load<Mesh>(const std::string& path)
		{
			auto* pAsset = LoadMeshAsset(path);
			return MakeShared<Mesh>(pAsset);
		}

		/**
		 * \brief	加载Texture2D
		 * \param	path		Texture2D路径
		 */
		template <>
		static SharedPtr<Texture2D> Load<Texture2D>(const std::string& path)
		{
			return LoadTexture2D(path);
		}

		/**
		 * \brief	异步加载资源
		 * \param	path		资源路径
		 * \return	异步加载请求
		 */
		template <typename T>
		static ResourceAsyncRequestSptr LoadAsync(const std::string& path);

		/**
		 * \brief	异步加载Mesh
		 * \param	path		Mesh路径
		 * \return	异步加载请求
		 */
		template <>
		static ResourceAsyncRequestSptr LoadAsync<Mesh>(const std::string& path)
		{

		}
		
	private:
		/**
		 * \brief	所有异步加载请求列表
		 */
		static std::vector<ResourceAsyncRequestSptr> asyncRequests;
	};
}

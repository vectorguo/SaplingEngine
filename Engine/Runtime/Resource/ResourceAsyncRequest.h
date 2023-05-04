#pragma once

#include "Runtime/SaplingEnginePch.h"

namespace SaplingEngine
{
	class ResourceAsyncRequest
	{
	public:
		ResourceAsyncRequest() = default;
		~ResourceAsyncRequest() = default;

		/*
		 * 禁止拷贝和移动
		 */
		ResourceAsyncRequest(const ResourceAsyncRequest&) = delete;
		ResourceAsyncRequest(ResourceAsyncRequest&&) = delete;
		ResourceAsyncRequest& operator= (const ResourceAsyncRequest&) = delete;
		ResourceAsyncRequest& operator= (ResourceAsyncRequest&&) = delete;

		/**
		 * \brief	资源加载是否完成
		 */
		bool IsDone() const
		{
			return m_IsDone;
		}

		/**
		 * \brief	获取资源
		 * \return	资源的智能指针
		 */
		template <typename T>
		SharedPtr<T> GetResource() const
		{
			return StaticPointerCast<T>(m_ResourceSptr);
		}

	private:
		/**
		 * \brief	资源加载是否完成
		 */
		bool m_IsDone = false;

		/**
		 * \brief	资源智能指针
		 */
		ObjectSptr m_ResourceSptr;
	};

	using ResourceAsyncRequestSptr = SharedPtr<ResourceAsyncRequest>;
}
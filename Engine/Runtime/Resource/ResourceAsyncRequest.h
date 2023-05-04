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
		 * ��ֹ�������ƶ�
		 */
		ResourceAsyncRequest(const ResourceAsyncRequest&) = delete;
		ResourceAsyncRequest(ResourceAsyncRequest&&) = delete;
		ResourceAsyncRequest& operator= (const ResourceAsyncRequest&) = delete;
		ResourceAsyncRequest& operator= (ResourceAsyncRequest&&) = delete;

		/**
		 * \brief	��Դ�����Ƿ����
		 */
		bool IsDone() const
		{
			return m_IsDone;
		}

		/**
		 * \brief	��ȡ��Դ
		 * \return	��Դ������ָ��
		 */
		template <typename T>
		SharedPtr<T> GetResource() const
		{
			return StaticPointerCast<T>(m_ResourceSptr);
		}

	private:
		/**
		 * \brief	��Դ�����Ƿ����
		 */
		bool m_IsDone = false;

		/**
		 * \brief	��Դ����ָ��
		 */
		ObjectSptr m_ResourceSptr;
	};

	using ResourceAsyncRequestSptr = SharedPtr<ResourceAsyncRequest>;
}
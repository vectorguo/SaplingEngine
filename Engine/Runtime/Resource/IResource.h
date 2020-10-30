#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	/**
	 * \brief 资源类型
	 */
	enum class EResourceType
	{
		/**
		 * \brief Mesh
		 */
		Mesh,
	};
	
	class IResource
	{
	protected:
		explicit IResource(EResourceType resourceType)
			:m_ResourceType(resourceType)
		{
			
		}
		
	public:
		virtual ~IResource() = default;
		
		/*
		 * 禁止拷贝和移动
		 */
		IResource(const IResource&) = delete;
		IResource(IResource&&) = delete;
		IResource& operator= (const IResource&) = delete;
		IResource& operator= (IResource&&) = delete;
		
		/**
		 * \brief 获取资源类型
		 * \return 资源类型
		 */
		EResourceType GetResourceType() const
		{
			return m_ResourceType;
		}
		
	private:
		/**
		 * \brief 资源类型
		 */
		EResourceType m_ResourceType;
	};

	using IResourceSptr = std::shared_ptr<IResource>;
}
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
		IResource() = default;
		
	public:
		virtual ~IResource() = default;
		
		/*
		 * 禁止拷贝和移动
		 */
		IResource(const IResource&) = delete;
		IResource(IResource&&) = delete;
		IResource& operator= (const IResource&) = delete;
		IResource& operator= (IResource&&) = delete;
	};

	using IResourceSptr = std::shared_ptr<IResource>;
}
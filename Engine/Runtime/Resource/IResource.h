#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class IResource
	{
	public:
		IResource() = default;
		virtual ~IResource() = default;
		
		/*
		 * ½ûÖ¹¿½±´ºÍÒÆ¶¯
		 */
		IResource(const IResource&) = delete;
		IResource(IResource&&) = delete;
		IResource& operator= (const IResource&) = delete;
		IResource& operator= (IResource&&) = delete;
		
	private:
		
	};

	using IResourceSptr = std::shared_ptr<IResource>;
}
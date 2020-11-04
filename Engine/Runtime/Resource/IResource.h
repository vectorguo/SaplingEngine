#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	/**
	 * \brief ��Դ����
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
		 * ��ֹ�������ƶ�
		 */
		IResource(const IResource&) = delete;
		IResource(IResource&&) = delete;
		IResource& operator= (const IResource&) = delete;
		IResource& operator= (IResource&&) = delete;
	};

	using IResourceSptr = std::shared_ptr<IResource>;
}
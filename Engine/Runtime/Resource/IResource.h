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
		explicit IResource(EResourceType resourceType)
			:m_ResourceType(resourceType)
		{
			
		}
		
	public:
		virtual ~IResource() = default;
		
		/*
		 * ��ֹ�������ƶ�
		 */
		IResource(const IResource&) = delete;
		IResource(IResource&&) = delete;
		IResource& operator= (const IResource&) = delete;
		IResource& operator= (IResource&&) = delete;
		
		/**
		 * \brief ��ȡ��Դ����
		 * \return ��Դ����
		 */
		EResourceType GetResourceType() const
		{
			return m_ResourceType;
		}
		
	private:
		/**
		 * \brief ��Դ����
		 */
		EResourceType m_ResourceType;
	};

	using IResourceSptr = std::shared_ptr<IResource>;
}
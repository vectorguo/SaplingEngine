#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class RenderPass
	{
	protected:
		explicit RenderPass(const std::string& name);
		explicit RenderPass(std::string&& name);

	public:
		virtual ~RenderPass();

		RenderPass(const RenderPass&) = delete;
		RenderPass(RenderPass&&) = delete;
		RenderPass& operator=(const RenderPass&) = delete;
		RenderPass& operator=(RenderPass&&) = delete;

		/**
		 * \brief ��ȡRenderPass����
		 * \return RenderPass����
		 */
		const std::string& GetName() const
		{
			return m_Name;
		}

		/**
		 * \brief ��ȡ���ȼ�
		 * \return ���ȼ�
		 */
		int32_t GetPriority() const
		{
			return m_Priority;
		}
		
	private:
		/**
		 * \brief RenderPass����
		 */
		std::string m_Name;

		/**
		 * \brief RenderPass����Ⱦ���ȼ�
		 */
		int32_t m_Priority;
	};
}

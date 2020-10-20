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
		 * \brief 获取RenderPass名称
		 * \return RenderPass名称
		 */
		const std::string& GetName() const
		{
			return m_Name;
		}

		/**
		 * \brief 获取优先级
		 * \return 优先级
		 */
		int32_t GetPriority() const
		{
			return m_Priority;
		}
		
	private:
		/**
		 * \brief RenderPass名称
		 */
		std::string m_Name;

		/**
		 * \brief RenderPass的渲染优先级
		 */
		int32_t m_Priority;
	};
}

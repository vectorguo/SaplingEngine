#pragma once

#include "SaplingEnginePch.h"
#include "RenderPass/RenderPass.h"

namespace SaplingEngine
{
	class RenderPass;
	
	class RenderPipeline final
	{
	public:
		RenderPipeline() = default;
		~RenderPipeline();

		SINGLETON(RenderPipeline)

		/**
		 * \brief 开始初始化
		 */
		void BeginInitialize(HWND hWnd);

		/**
		 * \brief 结束初始化
		 */
		void EndInitialize();

		/**
		 * \brief 销毁
		 */
		void Destroy();
		
		/**
		 * \brief 添加RenderPass
		 * \param renderPassPtr RenderPass的unique_ptr
		 */
		void AddRenderPass(std::unique_ptr<RenderPass>&& renderPassPtr);

		/**
		 * \brief 删除RenderPass
		 * \param renderPassName RenderPass名称
		 */
		void RemoveRenderPass(const std::string& renderPassName);
		
	private:
		/**
		 * \brief 根据优先级对RenderPass进行排序
		 */
		void SortRenderPass();
		
	private:
		std::vector<std::unique_ptr<RenderPass>> m_RenderPasses;
	};
}

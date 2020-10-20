#include "RenderPipeline.h"

#include "Graphics/DirectX12/D3D12GraphicsManager.h"

namespace SaplingEngine
{
	RenderPipeline::~RenderPipeline() = default;

	/**
	 * \brief 开始初始化
	 */
	void RenderPipeline::BeginInitialize(HWND hWnd)
	{
		D3D12GraphicsManager::Instance()->BeginInitialize(hWnd);
	}

	/**
	 * \brief 结束初始化
	 */
	void RenderPipeline::EndInitialize()
	{
		D3D12GraphicsManager::Instance()->EndInitialize();
	}

	/**
	 * \brief 销毁
	 */
	void RenderPipeline::Destroy()
	{
		D3D12GraphicsManager::Instance()->Destroy();
	}

	/**
	 * \brief 添加RenderPass
	 * \param renderPassPtr RenderPass的unique_ptr
	 */
	void RenderPipeline::AddRenderPass(std::unique_ptr<RenderPass>&& renderPassPtr)
	{
		const auto iter = std::find_if(m_RenderPasses.begin(), m_RenderPasses.end(), [&renderPassPtr](const std::unique_ptr<RenderPass>& rp)
			{
				return rp->GetName() == renderPassPtr->GetName();
			});
		if (iter == m_RenderPasses.end())
		{
			m_RenderPasses.push_back(std::move(renderPassPtr));
		}
	}
	
	/**
	 * \brief 删除RenderPass
	 * \param renderPassName RenderPass名称
	 */
	void RenderPipeline::RemoveRenderPass(const std::string& renderPassName)
	{
		const auto iter = std::find_if(m_RenderPasses.begin(), m_RenderPasses.end(), [&renderPassName](const std::unique_ptr<RenderPass>& rp)
			{
				return rp->GetName() == renderPassName;
			});
		if (iter != m_RenderPasses.end())
		{
			m_RenderPasses.erase(iter);
		}
	}

	/**
	 * \brief 根据优先级对RenderPass进行排序
	 */
	void RenderPipeline::SortRenderPass()
	{
		std::sort(m_RenderPasses.begin(), m_RenderPasses.end(), [](const std::unique_ptr<RenderPass>& rp1, const std::unique_ptr<RenderPass>& rp2)
			{
				return rp1->GetPriority() < rp2->GetPriority();
			});
	}
}

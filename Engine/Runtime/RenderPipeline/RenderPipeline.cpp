#include "RenderPipeline.h"

#include "Application/GameSetting.h"
#include "Graphics/Mesh.h"

namespace SaplingEngine
{
	RenderPipeline::RenderPipeline() = default;
	RenderPipeline::~RenderPipeline() = default;

	/**
	 * \brief 开始初始化
	 */
	void RenderPipeline::BeginInitialize(HWND hWnd)
	{
		m_ScreenWidth = GameSetting::Instance()->ScreenWidth();
		m_ScreenHeight = GameSetting::Instance()->ScreenHeight();
		m_GraphicsManager.BeginInitializeDxEnvironment(hWnd, m_ScreenWidth, m_ScreenHeight);
	}

	/**
	 * \brief 结束初始化
	 */
	void RenderPipeline::EndInitialize()
	{
		m_GraphicsManager.EndInitializeDxEnvironment(m_ScreenWidth, m_ScreenHeight);
	}

	/**
	 * \brief 渲染
	 */
	void RenderPipeline::Render()
	{
		PreRender();
		m_GraphicsManager.Render();
		PostRender();
	}

	/**
	 * \brief 销毁
	 */
	void RenderPipeline::Destroy()
	{
		m_GraphicsManager.Destroy();
	}

	/**
	 * \brief 设置宽度和高度
	 */
	void RenderPipeline::Resize()
	{
		const auto width = GameSetting::Instance()->ScreenWidth();
		const auto height = GameSetting::Instance()->ScreenHeight();
		if (width != m_ScreenWidth || height != m_ScreenHeight)
		{
			m_ScreenWidth = width;
			m_ScreenHeight = height;
			m_GraphicsManager.Resize(m_ScreenWidth, m_ScreenHeight);
		}
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
	 * \brief 执行渲染前的准备工作
	 */
	void RenderPipeline::PreRender()
	{
		m_GraphicsManager.PreRender();

		//上传Mesh数据
		Mesh::UploadMeshDatas();
	}

	/**
	 * \brief 执行渲染后的清理工作
	 */
	void RenderPipeline::PostRender()
	{
		m_GraphicsManager.PostRender();
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

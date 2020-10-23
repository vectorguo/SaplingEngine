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
	void RenderPipeline::BeginInitialize()
	{
		m_ScreenWidth = GameSetting::Instance()->ScreenWidth();
		m_ScreenHeight = GameSetting::Instance()->ScreenHeight();

		//Graphics初始化
		m_GraphicsManager.CreateDevice();
		m_GraphicsManager.CreateDescriptorHeaps();

		//Command初始化
		m_CommandManager.Initialize(m_GraphicsManager.m_D3D12Device.Get());
	}

	/**
	 * \brief 结束初始化
	 */
	void RenderPipeline::EndInitialize(HWND hWnd)
	{
		m_GraphicsManager.CreateSwapChain(hWnd, m_ScreenWidth, m_ScreenHeight);
		m_GraphicsManager.CreateRootSignature();
		m_GraphicsManager.CreatePipelineState();
		m_GraphicsManager.CreateRtv();
		m_GraphicsManager.CreateDsv(m_ScreenWidth, m_ScreenHeight);
		m_GraphicsManager.CreateCbv();
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
		m_CommandManager.Destroy();
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
		m_CommandManager.ResetCommandAllocator();
		m_CommandManager.ResetCommandList(m_GraphicsManager.m_PipelineState.Get());

		//设置ViewPort和ScissorRect
		m_CommandManager.SetViewports(&m_GraphicsManager.m_Viewport);
		m_CommandManager.SetScissorRects(&m_GraphicsManager.m_ScissorRect);

		//执行缓存的资源转换
		auto& transitions = m_CommandManager.m_ResourceBarrierTransitions;
		for (auto iter = transitions.begin(); iter != transitions.end(); ++iter)
		{
			m_CommandManager.m_CommandList->ResourceBarrier(1, &(*iter));
		}
		transitions.clear();

		//渲染缓存从呈现状态切换到RT状态
		m_CommandManager.ResourceBarrierTransition(m_GraphicsManager.CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		
		//上传Mesh数据
		Mesh::UploadMeshDatas();
	}

	/**
	 * \brief 执行渲染后的清理工作
	 */
	void RenderPipeline::PostRender()
	{
		//渲染缓存从RT状态切换到呈现状态
		m_CommandManager.ResourceBarrierTransition(m_GraphicsManager.CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_CommandManager.ExecuteCommandList();

		//交换后台缓冲和前台缓冲
		m_GraphicsManager.Present();

		//等待命令结束
		m_CommandManager.CompleteCommand();
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

#include "RenderPipeline.h"

#include "Application/GameSetting.h"
#include "Graphics/Mesh.h"

namespace SaplingEngine
{
	RenderPipeline::RenderPipeline() = default;
	RenderPipeline::~RenderPipeline() = default;

	/**
	 * \brief ��ʼ��ʼ��
	 */
	void RenderPipeline::BeginInitialize()
	{
		m_ScreenWidth = GameSetting::Instance()->ScreenWidth();
		m_ScreenHeight = GameSetting::Instance()->ScreenHeight();

		//Graphics��ʼ��
		m_GraphicsManager.CreateDevice();
		m_GraphicsManager.CreateDescriptorHeaps();

		//Command��ʼ��
		m_CommandManager.Initialize(m_GraphicsManager.m_D3D12Device.Get());
	}

	/**
	 * \brief ������ʼ��
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
	 * \brief ��Ⱦ
	 */
	void RenderPipeline::Render()
	{
		PreRender();
		m_GraphicsManager.Render();
		PostRender();
	}

	/**
	 * \brief ����
	 */
	void RenderPipeline::Destroy()
	{
		m_CommandManager.Destroy();
		m_GraphicsManager.Destroy();
	}

	/**
	 * \brief ���ÿ�Ⱥ͸߶�
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
	 * \brief ���RenderPass
	 * \param renderPassPtr RenderPass��unique_ptr
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
	 * \brief ɾ��RenderPass
	 * \param renderPassName RenderPass����
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
	 * \brief ִ����Ⱦǰ��׼������
	 */
	void RenderPipeline::PreRender()
	{
		m_CommandManager.ResetCommandAllocator();
		m_CommandManager.ResetCommandList(m_GraphicsManager.m_PipelineState.Get());

		//����ViewPort��ScissorRect
		m_CommandManager.SetViewports(&m_GraphicsManager.m_Viewport);
		m_CommandManager.SetScissorRects(&m_GraphicsManager.m_ScissorRect);

		//ִ�л������Դת��
		auto& transitions = m_CommandManager.m_ResourceBarrierTransitions;
		for (auto iter = transitions.begin(); iter != transitions.end(); ++iter)
		{
			m_CommandManager.m_CommandList->ResourceBarrier(1, &(*iter));
		}
		transitions.clear();

		//��Ⱦ����ӳ���״̬�л���RT״̬
		m_CommandManager.ResourceBarrierTransition(m_GraphicsManager.CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		
		//�ϴ�Mesh����
		Mesh::UploadMeshDatas();
	}

	/**
	 * \brief ִ����Ⱦ���������
	 */
	void RenderPipeline::PostRender()
	{
		//��Ⱦ�����RT״̬�л�������״̬
		m_CommandManager.ResourceBarrierTransition(m_GraphicsManager.CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_CommandManager.ExecuteCommandList();

		//������̨�����ǰ̨����
		m_GraphicsManager.Present();

		//�ȴ��������
		m_CommandManager.CompleteCommand();
	}

	/**
	 * \brief �������ȼ���RenderPass��������
	 */
	void RenderPipeline::SortRenderPass()
	{
		std::sort(m_RenderPasses.begin(), m_RenderPasses.end(), [](const std::unique_ptr<RenderPass>& rp1, const std::unique_ptr<RenderPass>& rp2)
			{
				return rp1->GetPriority() < rp2->GetPriority();
			});
	}
}

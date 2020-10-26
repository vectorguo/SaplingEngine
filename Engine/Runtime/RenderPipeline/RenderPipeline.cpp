#include "RenderPipeline.h"

#include "Application/GameSetting.h"
#include "Graphics/Mesh.h"
#include "RenderLibrary/DirectX12/Dx12CommandManager.h"
#include "RenderLibrary/DirectX12/Dx12GraphicsManager.h"

namespace SaplingEngine
{
	RenderPipeline::RenderPipeline() = default;
	
	RenderPipeline::~RenderPipeline()
	{
		delete m_pCommandManager;
		delete m_pGraphicsManager;
	}

	/**
	 * \brief ��ʼ��ʼ��
	 */
	void RenderPipeline::BeginInitialize(HWND hWnd)
	{
		m_ScreenWidth = GameSetting::Instance()->ScreenWidth();
		m_ScreenHeight = GameSetting::Instance()->ScreenHeight();

		//��������ʼ��Graphics Manager
		m_pGraphicsManager = new Dx12GraphicsManager();
		m_pGraphicsManager->BeginInitialize(hWnd, m_ScreenWidth, m_ScreenHeight);

		//��������ʼ��Command Manager
		m_pCommandManager = new Dx12CommandManager();
		m_pCommandManager->BeginInitialize();
	}

	/**
	 * \brief ������ʼ��
	 */
	void RenderPipeline::EndInitialize(HWND hWnd)
	{
		m_pGraphicsManager->EndInitialize(hWnd, m_ScreenWidth, m_ScreenHeight);
		m_pCommandManager->EndInitialize();  
	}

	/**
	 * \brief ��Ⱦ
	 */
	void RenderPipeline::Render()
	{
		PreRender();
		m_pGraphicsManager->Render();
		PostRender();
	}

	/**
	 * \brief ����
	 */
	void RenderPipeline::Destroy()
	{
		m_pCommandManager->Destroy();
		delete m_pCommandManager;
		m_pCommandManager = nullptr;
		
		m_pGraphicsManager->Destroy();
		delete m_pGraphicsManager;
		m_pGraphicsManager = nullptr;
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
			m_pGraphicsManager->Resize(m_ScreenWidth, m_ScreenHeight);
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
		m_pCommandManager->PreRender();
		
		//�ϴ�Mesh����
		Mesh::UploadMeshDatas();
	}

	/**
	 * \brief ִ����Ⱦ���������
	 */
	void RenderPipeline::PostRender()
	{
		m_pCommandManager->PostRender();
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

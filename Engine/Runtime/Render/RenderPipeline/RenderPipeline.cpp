#include "RenderPipeline.h"

#include "Application/GameSetting.h"
#include "Camera/CameraManager.h"
#include "Render/Graphics/Mesh.h"
#include "Render/Graphics/DirectX12/Dx12CommandManager.h"
#include "Render/Graphics/DirectX12/Dx12GraphicsManager.h"
#include "RenderPass/RenderOpaquePass.h"
#include "Scene/SceneManager.h"

namespace SaplingEngine
{
	RenderPipeline::RenderPipeline() = default;
	RenderPipeline::~RenderPipeline() = default;

	/**
	 * \brief ��ʼ��ʼ��
	 */
	void RenderPipeline::BeginInitialize(HWND hWnd)
	{
		m_ScreenWidth = GameSetting::ScreenWidth();
		m_ScreenHeight = GameSetting::ScreenHeight();

		//��������ʼ��Graphics Manager
		m_pGraphicsManager = new Dx12GraphicsManager();
		m_pGraphicsManager->BeginInitialize(hWnd, m_ScreenWidth, m_ScreenHeight);

		//��������ʼ������������������
		m_pConstantBufferManager = new Dx12CBufferManager();
		m_pConstantBufferManager->Initialize();

		//��������ʼ��Command Manager
		m_pCommandManager = new Dx12CommandManager();
		m_pCommandManager->BeginInitialize();

		//��ʼ����Ⱦ����
		auto* pOpaquePass = new RenderOpaquePass("Render Opaque");
		pOpaquePass->SetBackgroundColor(Color::LightBlue);
		m_RenderPasses.push_back(pOpaquePass);
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

		auto* pActiveScene = SceneManager::Instance()->GetActiveScene();

		//����Object���ݳ���������
		m_pConstantBufferManager->UpdateOcbData(pActiveScene);
		
		//ִ��Render Pass
		const auto& cameras = CameraManager::GetCameras();
		for (const auto& pCamera : cameras)
		{
			//����Pass���ݳ���������
			m_pConstantBufferManager->UpdatePcbData(pCamera.get());
			for (auto iter = m_RenderPasses.begin(); iter != m_RenderPasses.end(); ++iter)
			{
				(*iter)->Render(pCamera.get(), pActiveScene);
			}
		}
		
		PostRender();
	}

	/**
	 * \brief ����
	 */
	void RenderPipeline::Destroy()
	{
		for (auto iter = m_RenderPasses.begin(); iter != m_RenderPasses.end(); ++iter)
		{
			delete* iter;
		}
		m_RenderPasses.clear();
		
		m_pCommandManager->Destroy();
		delete m_pCommandManager;
		m_pCommandManager = nullptr;

		m_pConstantBufferManager->Destroy();
		delete m_pConstantBufferManager;
		m_pConstantBufferManager = nullptr;
		
		m_pGraphicsManager->Destroy();
		delete m_pGraphicsManager;
		m_pGraphicsManager = nullptr;
	}

	/**
	 * \brief ���ÿ�Ⱥ͸߶�
	 * \param width ��Ļ���
	 * \param height ��Ļ�߶�
	 */
	void RenderPipeline::OnSceneResize(uint32_t width, uint32_t height)
	{
		if (width != m_ScreenWidth || height != m_ScreenHeight)
		{
			m_ScreenWidth = width;
			m_ScreenHeight = height;
			m_pGraphicsManager->OnWindowResize(m_ScreenWidth, m_ScreenHeight);
		}
	}

	/**
	 * \brief ���RenderPass
	 * \param pRenderPass ��Ⱦ����ָ��,Add֮����RenderPipeline����ÿ��Pass����������
	 */
	void RenderPipeline::AddRenderPass(RenderPass* pRenderPass)
	{
		const auto iter = std::find_if(m_RenderPasses.begin(), m_RenderPasses.end(), [pRenderPass](const RenderPass* rp)
			{
				return rp->GetName() == pRenderPass->GetName();
			});
		if (iter == m_RenderPasses.end())
		{
			m_RenderPasses.push_back(pRenderPass);
		}
	}
	
	/**
	 * \brief ɾ��RenderPass
	 * \param renderPassName RenderPass����
	 */
	void RenderPipeline::RemoveRenderPass(const std::string& renderPassName)
	{
		const auto iter = std::find_if(m_RenderPasses.begin(), m_RenderPasses.end(), [&renderPassName](const RenderPass* rp)
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
		std::sort(m_RenderPasses.begin(), m_RenderPasses.end(), [](const RenderPass* rp1, const RenderPass* rp2)
			{
				return rp1->GetPriority() < rp2->GetPriority();
			});
	}
}

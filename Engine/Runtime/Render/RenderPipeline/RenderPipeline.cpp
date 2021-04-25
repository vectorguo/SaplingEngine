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
	uint32_t					RenderPipeline::screenWidth = 0;
	uint32_t					RenderPipeline::screenHeight = 0;
	std::vector<RenderPass*>	RenderPipeline::renderPasses;

	/**
	 * \brief ��ʼ��ʼ��
	 */
	void RenderPipeline::BeginInitialize(HWND hWnd)
	{
		screenWidth = GameSetting::ScreenWidth();
		screenHeight = GameSetting::ScreenHeight();

		//��������ʼ��Graphics Manager
		GraphicsManager::BeginInitialize(hWnd, screenWidth, screenHeight);

		//��������ʼ������������������
		CBufferManager::Initialize();

		//��������ʼ��Command Manager
		CommandManager::BeginInitialize();

		//��ʼ����Ⱦ����
		auto* pOpaquePass = new RenderOpaquePass("Render Opaque");
		pOpaquePass->SetBackgroundColor(Color::LightBlue);
		renderPasses.push_back(pOpaquePass);
	}

	/**
	 * \brief ������ʼ��
	 */
	void RenderPipeline::EndInitialize(HWND hWnd)
	{
		GraphicsManager::EndInitialize(hWnd, screenWidth, screenHeight);
		CommandManager::EndInitialize();
	}

	/**
	 * \brief ��Ⱦ
	 */
	void RenderPipeline::Render()
	{
		PreRender();

		auto* pActiveScene = SceneManager::Instance()->GetActiveScene();

		//����Object���ݳ���������
		CBufferManager::UpdateOcbData(pActiveScene);
		
		//ִ��Render Pass
		const auto& cameras = CameraManager::GetCameras();
		for (const auto& pCamera : cameras)
		{
			//����Pass���ݳ���������
			CBufferManager::UpdatePcbData(pCamera.get());
			for (auto iter = renderPasses.begin(); iter != renderPasses.end(); ++iter)
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
		for (auto iter = renderPasses.begin(); iter != renderPasses.end(); ++iter)
		{
			delete* iter;
		}
		renderPasses.clear();
		
		CommandManager::Destroy();
		CBufferManager::Destroy();
		GraphicsManager::Destroy();
	}

	/**
	 * \brief ���ÿ�Ⱥ͸߶�
	 * \param width ��Ļ���
	 * \param height ��Ļ�߶�
	 */
	void RenderPipeline::OnSceneResize(uint32_t width, uint32_t height)
	{
		if (width != screenWidth || height != screenHeight)
		{
			screenWidth = width;
			screenHeight = height;
			GraphicsManager::OnWindowResize(screenWidth, screenHeight);
		}
	}

	/**
	 * \brief ���RenderPass
	 * \param pRenderPass ��Ⱦ����ָ��,Add֮����RenderPipeline����ÿ��Pass����������
	 */
	void RenderPipeline::AddRenderPass(RenderPass* pRenderPass)
	{
		const auto iter = std::find_if(renderPasses.begin(), renderPasses.end(), [pRenderPass](const RenderPass* rp)
			{
				return rp->GetName() == pRenderPass->GetName();
			});
		if (iter == renderPasses.end())
		{
			renderPasses.push_back(pRenderPass);
		}
	}
	
	/**
	 * \brief ɾ��RenderPass
	 * \param renderPassName RenderPass����
	 */
	void RenderPipeline::RemoveRenderPass(const std::string& renderPassName)
	{
		const auto iter = std::find_if(renderPasses.begin(), renderPasses.end(), [&renderPassName](const RenderPass* rp)
			{
				return rp->GetName() == renderPassName;
			});
		if (iter != renderPasses.end())
		{
			renderPasses.erase(iter);
		}
	}

	/**
	 * \brief ִ����Ⱦǰ��׼������
	 */
	void RenderPipeline::PreRender()
	{
		CommandManager::PreRender();
		
		//�ϴ�Mesh����
		Mesh::UploadMeshDatas();
	}

	/**
	 * \brief ִ����Ⱦ���������
	 */
	void RenderPipeline::PostRender()
	{
		CommandManager::PostRender();
	}

	/**
	 * \brief �������ȼ���RenderPass��������
	 */
	void RenderPipeline::SortRenderPass()
	{
		std::sort(renderPasses.begin(), renderPasses.end(), [](const RenderPass* rp1, const RenderPass* rp2)
			{
				return rp1->GetPriority() < rp2->GetPriority();
			});
	}
}

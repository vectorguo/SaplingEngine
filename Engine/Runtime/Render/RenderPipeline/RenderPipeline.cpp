#include "Runtime/Application/Setting.h"
#include "Runtime/Camera/CameraManager.h"
#include "Runtime/Render/Graphics/ConstantBufferData.h"
#include "Runtime/Render/Graphics/Mesh/MeshManager.h"
#include "Runtime/Render/Graphics/Texture/TextureManager.h"
#include "Runtime/Render/RenderPipeline/RenderPipeline.h"
#include "Runtime/Render/RenderPipeline/RenderPass/RenderOpaquePass.h"
#include "Runtime/Render/RenderPipeline/RenderPass/ShadowPass.h"
#include "Runtime/Scene/SceneManager.h"

namespace SaplingEngine
{
	uint32_t					RenderPipeline::screenWidth = 0;
	uint32_t					RenderPipeline::screenHeight = 0;
	std::map<size_t, RenderPipeline::RenderItemContainer> RenderPipeline::renderItemsMap;
	std::vector<RenderPass*>	RenderPipeline::renderPasses;
	RenderOpaquePass*			RenderPipeline::opaquePassPtr = nullptr;
	ShadowPass*					RenderPipeline::shadowPassPtr = nullptr;

	/**
	 * \brief	��ʼ��
	 */
	void RenderPipeline::Initialize(HWND hWnd)
	{
		screenWidth = Setting::ScreenWidth();
		screenHeight = Setting::ScreenHeight();

		//����DirectX�豸
		GraphicsManager::CreateDevice();

		//��������ʼ��Command Manager
		CommandManager::BeginInitialize();

		//��������ʼ��Graphics Manager
		GraphicsManager::Initialize(hWnd, screenWidth, screenHeight);

		//��������������ʼ��
		DescriptorManager::Initialize();

		//��ʼ����Ⱦ����
		opaquePassPtr = new RenderOpaquePass("Render Opaque");
		opaquePassPtr->SetPriority(10);
		opaquePassPtr->SetBackgroundColor(Color::LightBlue);
		renderPasses.push_back(opaquePassPtr);

		shadowPassPtr = new ShadowPass("ShadowCaster");
		shadowPassPtr->SetPriority(1);
		renderPasses.push_back(shadowPassPtr);

		//������Ⱦ����
		SortRenderPass();

		CommandManager::EndInitialize();
	}

	/**
	 * \brief	��Ⱦ
	 */
	void RenderPipeline::Render()
	{
		PreRender();

		//ִ��Render Pass
		const auto& cameras = CameraManager::GetCameras();
		for (auto* pCamera : cameras)
		{
			if (pCamera->IsEnabled())
			{
				//����Pass��������������
				UploadPassCbvData(pCamera);

				//��ȾPass
				for (auto iter = renderPasses.begin(); iter != renderPasses.end(); ++iter)
				{
					auto* pRenderPass = *iter;
					if (pRenderPass->IsActive())
					{
						pRenderPass->Render();
					}
				}
			}
		}
		
		PostRender();


	}

	/**
	 * \brief	����
	 */
	void RenderPipeline::Destroy()
	{
		for (auto iter = renderPasses.begin(); iter != renderPasses.end(); ++iter)
		{
			delete* iter;
		}
		renderPasses.clear();
		
		CommandManager::Destroy();
		DescriptorManager::Destroy();
		GraphicsManager::Destroy();
	}

	/**
	 * \brief	���ÿ�Ⱥ͸߶�
	 * \param	width			��Ļ���
	 * \param	height			��Ļ�߶�
	 */
	void RenderPipeline::OnSceneResize(uint32_t width, uint32_t height)
	{
		if (width != screenWidth || height != screenHeight)
		{
			screenWidth = width;
			screenHeight = height;
			GraphicsManager::OnWindowResize(screenWidth, screenHeight);

			//for (auto iter = renderPasses.begin(); iter != renderPasses.end(); ++iter)
			//{
			//	(*iter)->OnSceneResize(screenWidth, screenHeight);
			//}
		}
	}

	/**
	 * \brief	���RenderPass
	 * \param	pRenderPass		��Ⱦ����ָ��,Add֮����RenderPipeline����ÿ��Pass����������
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
			SortRenderPass();
		}
	}
	
	/**
	 * \brief	ɾ��RenderPass
	 * \param	renderPassName	RenderPass����
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
	 * \brief	�������ȼ���RenderPass��������
	 */
	void RenderPipeline::SortRenderPass()
	{
		std::sort(renderPasses.begin(), renderPasses.end(), [](const RenderPass* rp1, const RenderPass* rp2)
			{
				return rp1->GetPriority() < rp2->GetPriority();
			});
	}

	/**
	 * \brief	��ʼ��Ⱦ
	 * \param	pCamera			���ָ��
	 */
	void RenderPipeline::BeginCameraRendering(const Camera* pCamera)
	{
	}

	/**
	 * \brief	��Ⱦ�����
	 * \param	pCamera			���ָ��
	 */
	void RenderPipeline::RenderSingleCamera(const Camera* pCamera)
	{
	}

	/**
	 * \brief	������Ⱦ
	 * \param	pCamera			���ָ��
	 */
	void RenderPipeline::EndCameraRendering(const Camera* pCamera)
	{
	}

	/**
	 * \brief	ִ����Ⱦǰ��׼������
	 */
	void RenderPipeline::PreRender()
	{
		CommandManager::PreRender();
		
		//�ϴ��´�����Mesh����
		MeshManager::UploadMeshDatas();

		//������ͼ��Դ
		TextureManager::UploadTextureDatas();

		//����Object��������������
		UploadObjectCbvData();

		//ִ����Ⱦǰ��׼������
		for (auto iter = renderPasses.begin(); iter != renderPasses.end(); ++iter)
		{
			auto* pRenderPass = *iter;
			if (pRenderPass->IsActive())
			{
				pRenderPass->PreRender();
			}
		}
	}

	/**
	 * \brief	ִ����Ⱦ���������
	 */
	void RenderPipeline::PostRender()
	{
		//ִ����Ⱦ���������
		for (auto iter = renderPasses.begin(); iter != renderPasses.end(); ++iter)
		{
			auto* pRenderPass = *iter;
			if (pRenderPass->IsActive())
			{
				pRenderPass->PostRender();
			}
		}

		CommandManager::PostRender();
	}

	/**
	 * \brief	�������峣������������
	 */
	void RenderPipeline::UploadObjectCbvData()
	{
		for (auto iter = renderItemsMap.begin(); iter != renderItemsMap.end(); ++iter)
		{
			iter->second.UploadRenderData();
		}
	}

	/**
	 * \brief	����Pass��������������
	 */
	void RenderPipeline::UploadPassCbvData(Camera* pCamera)
	{
		auto* pUploadBuffer = Dx12DescriptorManager::GetPassCbUploadBuffer();
		pUploadBuffer->CopyData(CommonPcbData::FillPcbData(pCamera), CommonPcbData::DataSize, 0);

		if (shadowPassPtr && shadowPassPtr->IsActive())
		{
			pUploadBuffer->CopyData(CommonPcbData::FillShadowPcbData(shadowPassPtr), CommonPcbData::ShadowDataSize, Dx12DescriptorManager::PassCommonCbSize);
		}
	}
}

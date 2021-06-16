#include "Application/Setting.h"
#include "Camera/CameraManager.h"
#include "Render/Graphics/ConstantBufferData.h"
#include "Render/Graphics/DirectX12/Dx12CommandManager.h"
#include "Render/Graphics/DirectX12/Dx12DescriptorManager.h"
#include "Render/Graphics/DirectX12/Dx12GraphicsManager.h"
#include "Render/Graphics/MeshManager.h"
#include "Render/Graphics/TextureManager.h"
#include "Render/RenderPipeline/RenderPipeline.h"
#include "Render/RenderPipeline/RenderPass/RenderOpaquePass.h"
#include "Render/RenderPipeline/RenderPass/ShadowPass.h"
#include "Scene/SceneManager.h"

namespace SaplingEngine
{
	uint32_t					RenderPipeline::screenWidth = 0;
	uint32_t					RenderPipeline::screenHeight = 0;
	std::map<size_t, std::vector<Renderer*>> RenderPipeline::renderItems;
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

		//��������������ʼ��
		DescriptorManager::Initialize();

		//��������ʼ��Graphics Manager
		GraphicsManager::Initialize(hWnd, screenWidth, screenHeight);

		//��������ʼ������������������
		BufferManager::Initialize();

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

		//����Object��������������
		UpdateObjectCbvData();

		//ִ��Render Pass
		const auto& cameras = CameraManager::GetCameras();
		for (auto* pCamera : cameras)
		{
			if (pCamera->IsEnabled())
			{
				//����Pass��������������
				UpdatePassCbvData(pCamera);

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
		BufferManager::Destroy();
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
	void RenderPipeline::UpdateObjectCbvData()
	{
		size_t specialDataSize;
		for (auto iter1 = renderItems.begin(); iter1 != renderItems.end(); ++iter1)
		{
			const auto& shaderHashValue = iter1->first;
			const auto& items = iter1->second;
			for (auto iter2 = items.begin(); iter2 != items.end(); ++iter2)
			{
				auto* pRenderer = *iter2;
				const auto* pCommonData = CommonOcbData::FillOcbData(pRenderer->GetTransform());
				const auto* pSpecialData = pRenderer->FillSpecialOcbData(specialDataSize, pRenderer->GetMaterial());
				BufferManager::FillOcbData(pRenderer->GetCbvIndex(), pCommonData, CommonOcbData::DataSize, pSpecialData, specialDataSize);
			}
		}
	}

	/**
	 * \brief	����Pass��������������
	 */
	void RenderPipeline::UpdatePassCbvData(Camera* pCamera)
	{
		BufferManager::FillPcbData(CommonPcbData::FillPcbData(pCamera), CommonPcbData::DataSize);

		if (shadowPassPtr && shadowPassPtr->IsActive())
		{
			BufferManager::FillShadowPcbData(CommonPcbData::FillShadowPcbData(shadowPassPtr), CommonPcbData::ShadowDataSize);
		}
	}
}

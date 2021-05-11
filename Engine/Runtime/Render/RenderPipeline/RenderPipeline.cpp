#include "RenderPipeline.h"

#include "Application/GameSetting.h"
#include "Camera/CameraManager.h"
#include "Render/Graphics/ConstantBufferData.h"
#include "Render/Graphics/DirectX12/Dx12CommandManager.h"
#include "Render/Graphics/DirectX12/Dx12GraphicsManager.h"
#include "Render/Graphics/MeshManager.h"
#include "Render/Graphics/TextureManager.h"
#include "Render/RenderPipeline/RenderPass/RenderOpaquePass.h"
#include "Scene/SceneManager.h"

namespace SaplingEngine
{
	uint32_t					RenderPipeline::screenWidth = 0;
	uint32_t					RenderPipeline::screenHeight = 0;
	std::map<size_t, std::vector<Renderer*>> RenderPipeline::renderItems;
	std::vector<RenderPass*>	RenderPipeline::renderPasses;

	/**
	 * \brief	��ʼ��ʼ��
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
	 * \brief	������ʼ��
	 */
	void RenderPipeline::EndInitialize(HWND hWnd)
	{
		GraphicsManager::EndInitialize(hWnd, screenWidth, screenHeight);
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
			//���³�������������
			UpdateCbvData(pCamera);

			//��ȾPass
			for (auto iter = renderPasses.begin(); iter != renderPasses.end(); ++iter)
			{
				(*iter)->Render(pCamera);
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
		CBufferManager::Destroy();
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
	 * \brief	ִ����Ⱦǰ��׼������
	 */
	void RenderPipeline::PreRender()
	{
		CommandManager::PreRender();
		
		//�ϴ��´�����Mesh����
		MeshManager::UploadMeshDatas();

		//������ͼ��Դ
		TextureManager::UploadTextureDatas();
	}

	/**
	 * \brief	ִ����Ⱦ���������
	 */
	void RenderPipeline::PostRender()
	{
		CommandManager::PostRender();
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
	 * \brief	���³�������������
	 */
	void RenderPipeline::UpdateCbvData(Camera* pCamera)
	{
		size_t specialDataSize;
		for (auto iter1 = renderItems.begin(); iter1 != renderItems.end(); ++iter1)
		{
			const auto& shaderName = iter1->first;
			const auto& items = iter1->second;
			for (auto iter2 = items.begin(); iter2 != items.end(); ++iter2)
			{
				auto* pRenderer = *iter2;
				const auto* pCommonData = CommonOcbData::FillOcbData(pRenderer->GetTransform());
				const auto* pSpecialData = pRenderer->FillSpecialOcbData(specialDataSize, pRenderer->GetMaterial());
				Dx12CBufferManager::FillOcbData(shaderName, pRenderer->GetCbvIndex(), pCommonData, CommonOcbData::DataSize, pSpecialData, specialDataSize);
			}

			Dx12CBufferManager::FillPcbData(shaderName, CommonPcbData::FillPcbData(pCamera), CommonPcbData::DataSize);
		}
	}
}

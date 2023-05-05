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
	 * \brief	初始化
	 */
	void RenderPipeline::Initialize(HWND hWnd)
	{
		screenWidth = Setting::ScreenWidth();
		screenHeight = Setting::ScreenHeight();

		//创建DirectX设备
		GraphicsManager::CreateDevice();

		//创建并初始化Command Manager
		CommandManager::BeginInitialize();

		//创建并初始化Graphics Manager
		GraphicsManager::Initialize(hWnd, screenWidth, screenHeight);

		//描述符管理器初始化
		DescriptorManager::Initialize();

		//初始化渲染管线
		opaquePassPtr = new RenderOpaquePass("Render Opaque");
		opaquePassPtr->SetPriority(10);
		opaquePassPtr->SetBackgroundColor(Color::LightBlue);
		renderPasses.push_back(opaquePassPtr);

		shadowPassPtr = new ShadowPass("ShadowCaster");
		shadowPassPtr->SetPriority(1);
		renderPasses.push_back(shadowPassPtr);

		//排序渲染管线
		SortRenderPass();

		CommandManager::EndInitialize();
	}

	/**
	 * \brief	渲染
	 */
	void RenderPipeline::Render()
	{
		PreRender();

		//执行Render Pass
		const auto& cameras = CameraManager::GetCameras();
		for (auto* pCamera : cameras)
		{
			if (pCamera->IsEnabled())
			{
				//更新Pass常量缓冲区数据
				UploadPassCbvData(pCamera);

				//渲染Pass
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
	 * \brief	销毁
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
	 * \brief	设置宽度和高度
	 * \param	width			屏幕宽度
	 * \param	height			屏幕高度
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
	 * \brief	添加RenderPass
	 * \param	pRenderPass		渲染管线指针,Add之后由RenderPipeline管理每个Pass的生命周期
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
	 * \brief	删除RenderPass
	 * \param	renderPassName	RenderPass名称
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
	 * \brief	根据优先级对RenderPass进行排序
	 */
	void RenderPipeline::SortRenderPass()
	{
		std::sort(renderPasses.begin(), renderPasses.end(), [](const RenderPass* rp1, const RenderPass* rp2)
			{
				return rp1->GetPriority() < rp2->GetPriority();
			});
	}

	/**
	 * \brief	开始渲染
	 * \param	pCamera			相机指针
	 */
	void RenderPipeline::BeginCameraRendering(const Camera* pCamera)
	{
	}

	/**
	 * \brief	渲染单相机
	 * \param	pCamera			相机指针
	 */
	void RenderPipeline::RenderSingleCamera(const Camera* pCamera)
	{
	}

	/**
	 * \brief	结束渲染
	 * \param	pCamera			相机指针
	 */
	void RenderPipeline::EndCameraRendering(const Camera* pCamera)
	{
	}

	/**
	 * \brief	执行渲染前的准备工作
	 */
	void RenderPipeline::PreRender()
	{
		CommandManager::PreRender();
		
		//上传新创建的Mesh数据
		MeshManager::UploadMeshDatas();

		//创建贴图资源
		TextureManager::UploadTextureDatas();

		//更新Object常量缓冲区数据
		UploadObjectCbvData();

		//执行渲染前的准备工作
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
	 * \brief	执行渲染后的清理工作
	 */
	void RenderPipeline::PostRender()
	{
		//执行渲染后的清理工作
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
	 * \brief	更新物体常量缓冲区数据
	 */
	void RenderPipeline::UploadObjectCbvData()
	{
		for (auto iter = renderItemsMap.begin(); iter != renderItemsMap.end(); ++iter)
		{
			iter->second.UploadRenderData();
		}
	}

	/**
	 * \brief	更新Pass常量缓冲区数据
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

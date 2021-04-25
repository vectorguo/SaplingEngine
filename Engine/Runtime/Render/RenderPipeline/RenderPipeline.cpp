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
	 * \brief 开始初始化
	 */
	void RenderPipeline::BeginInitialize(HWND hWnd)
	{
		screenWidth = GameSetting::ScreenWidth();
		screenHeight = GameSetting::ScreenHeight();

		//创建并初始化Graphics Manager
		GraphicsManager::BeginInitialize(hWnd, screenWidth, screenHeight);

		//创建并初始化常量缓冲区管理器
		CBufferManager::Initialize();

		//创建并初始化Command Manager
		CommandManager::BeginInitialize();

		//初始化渲染管线
		auto* pOpaquePass = new RenderOpaquePass("Render Opaque");
		pOpaquePass->SetBackgroundColor(Color::LightBlue);
		renderPasses.push_back(pOpaquePass);
	}

	/**
	 * \brief 结束初始化
	 */
	void RenderPipeline::EndInitialize(HWND hWnd)
	{
		GraphicsManager::EndInitialize(hWnd, screenWidth, screenHeight);
		CommandManager::EndInitialize();
	}

	/**
	 * \brief 渲染
	 */
	void RenderPipeline::Render()
	{
		PreRender();

		auto* pActiveScene = SceneManager::Instance()->GetActiveScene();

		//更新Object数据常量缓冲区
		CBufferManager::UpdateOcbData(pActiveScene);
		
		//执行Render Pass
		const auto& cameras = CameraManager::GetCameras();
		for (const auto& pCamera : cameras)
		{
			//更新Pass数据常量缓冲区
			CBufferManager::UpdatePcbData(pCamera.get());
			for (auto iter = renderPasses.begin(); iter != renderPasses.end(); ++iter)
			{
				(*iter)->Render(pCamera.get(), pActiveScene);
			}
		}
		
		PostRender();
	}

	/**
	 * \brief 销毁
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
	 * \brief 设置宽度和高度
	 * \param width 屏幕宽度
	 * \param height 屏幕高度
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
	 * \brief 添加RenderPass
	 * \param pRenderPass 渲染管线指针,Add之后由RenderPipeline管理每个Pass的生命周期
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
	 * \brief 删除RenderPass
	 * \param renderPassName RenderPass名称
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
	 * \brief 执行渲染前的准备工作
	 */
	void RenderPipeline::PreRender()
	{
		CommandManager::PreRender();
		
		//上传Mesh数据
		Mesh::UploadMeshDatas();
	}

	/**
	 * \brief 执行渲染后的清理工作
	 */
	void RenderPipeline::PostRender()
	{
		CommandManager::PostRender();
	}

	/**
	 * \brief 根据优先级对RenderPass进行排序
	 */
	void RenderPipeline::SortRenderPass()
	{
		std::sort(renderPasses.begin(), renderPasses.end(), [](const RenderPass* rp1, const RenderPass* rp2)
			{
				return rp1->GetPriority() < rp2->GetPriority();
			});
	}
}

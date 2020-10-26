#include "RenderPipeline.h"

#include "Application/GameSetting.h"
#include "Camera/CameraManager.h"
#include "Graphics/Mesh.h"
#include "RenderLibrary/DirectX12/Dx12CommandManager.h"
#include "RenderLibrary/DirectX12/Dx12GraphicsManager.h"
#include "RenderPass/RenderOpaquePass.h"
#include "Scene/SceneManager.h"

namespace SaplingEngine
{
	RenderPipeline::RenderPipeline() = default;
	
	RenderPipeline::~RenderPipeline()
	{
		delete m_pCommandManager;
		delete m_pGraphicsManager;
	}

	/**
	 * \brief 开始初始化
	 */
	void RenderPipeline::BeginInitialize(HWND hWnd)
	{
		m_ScreenWidth = GameSetting::Instance()->ScreenWidth();
		m_ScreenHeight = GameSetting::Instance()->ScreenHeight();

		//创建并初始化Graphics Manager
		m_pGraphicsManager = new Dx12GraphicsManager();
		m_pGraphicsManager->BeginInitialize(hWnd, m_ScreenWidth, m_ScreenHeight);

		//创建并初始化Command Manager
		m_pCommandManager = new Dx12CommandManager();
		m_pCommandManager->BeginInitialize();

		//初始化渲染管线
		auto* pOpaquePass = new RenderOpaquePass("Render Opaque");
		pOpaquePass->SetBackgroundColor(Color::LightBlue);
		m_RenderPasses.push_back(pOpaquePass);
	}

	/**
	 * \brief 结束初始化
	 */
	void RenderPipeline::EndInitialize(HWND hWnd)
	{
		m_pGraphicsManager->EndInitialize(hWnd, m_ScreenWidth, m_ScreenHeight);
		m_pCommandManager->EndInitialize();  
	}

	/**
	 * \brief 渲染
	 */
	void RenderPipeline::Render()
	{
		PreRender();

		//渲染
		const auto* pActiveScene = SceneManager::Instance()->GetActiveScene();
		const auto& cameras = CameraManager::Instance()->GetCameras();
		for (const auto& pCamera : cameras)
		{
			for (auto iter = m_RenderPasses.begin(); iter != m_RenderPasses.end(); ++iter)
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
		for (auto iter = m_RenderPasses.begin(); iter != m_RenderPasses.end(); ++iter)
		{
			delete* iter;
		}
		m_RenderPasses.clear();
		
		m_pCommandManager->Destroy();
		delete m_pCommandManager;
		m_pCommandManager = nullptr;
		
		m_pGraphicsManager->Destroy();
		delete m_pGraphicsManager;
		m_pGraphicsManager = nullptr;
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
			m_pGraphicsManager->Resize(m_ScreenWidth, m_ScreenHeight);
		}
	}

	/**
	 * \brief 添加RenderPass
	 * \param pRenderPass 渲染管线指针,Add之后由RenderPipeline管理每个Pass的生命周期
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
	 * \brief 删除RenderPass
	 * \param renderPassName RenderPass名称
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
	 * \brief 执行渲染前的准备工作
	 */
	void RenderPipeline::PreRender()
	{
		m_pCommandManager->PreRender();
		
		//上传Mesh数据
		Mesh::UploadMeshDatas();
	}

	/**
	 * \brief 执行渲染后的清理工作
	 */
	void RenderPipeline::PostRender()
	{
		m_pCommandManager->PostRender();
	}

	/**
	 * \brief 根据优先级对RenderPass进行排序
	 */
	void RenderPipeline::SortRenderPass()
	{
		std::sort(m_RenderPasses.begin(), m_RenderPasses.end(), [](const RenderPass* rp1, const RenderPass* rp2)
			{
				return rp1->GetPriority() < rp2->GetPriority();
			});
	}
}

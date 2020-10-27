#include "RenderPipeline.h"

#include "Application/GameSetting.h"
#include "Camera/CameraManager.h"
#include "Graphics/Mesh.h"
#include "RenderLibrary/DirectX12/Dx12CommandManager.h"
#include "RenderLibrary/DirectX12/Dx12GraphicsManager.h"
#include "RenderPass/RenderOpaquePass.h"
#include "Scene/Scene.h"
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

		//更新常量缓冲区
		auto* pActiveScene = SceneManager::Instance()->GetActiveScene();
		
		//执行Render Pass
		const auto& cameras = CameraManager::Instance()->GetCameras();
		for (const auto& pCamera : cameras)
		{
			//TODO PASSCB和OBJECTCB要拆分开
			UpdateConstantBuffer(pCamera.get(), pActiveScene);
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
	void RenderPipeline::OnWindowResize()
	{
		const auto width = GameSetting::Instance()->ScreenWidth();
		const auto height = GameSetting::Instance()->ScreenHeight();
		if (width != m_ScreenWidth || height != m_ScreenHeight)
		{
			m_ScreenWidth = width;
			m_ScreenHeight = height;
			m_pGraphicsManager->OnWindowResize(m_ScreenWidth, m_ScreenHeight);
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

	/**
	 * \brief 更新常量缓冲区
	 * \param pCamera 渲染使用的相机
	 * \param pActiveScene 当前活动场景
	 */
	void RenderPipeline::UpdateConstantBuffer(const Camera* pCamera, Scene* pActiveScene)
	{
		auto& objects = pActiveScene->GetGameObjects();
		for (auto iter = objects.begin(); iter != objects.end(); ++iter)
		{
			auto* pTransform = (*iter)->GetComponent<Transform>().get();
			
			// auto localToWorldMatrix = pTransform->GetLocalToWorldMatrix().Transpose();
			// auto worldToProjectMatrix = XM
			//
			// ObjectConstantData data;
			// data.ModelToWorldMatrix = Matrix4x4::Translate(0, 0, 1.0f);// Matrix4x4::Scale(0.5f, 0.5f, 0.5f);
			// data.ModelToWorldMatrix = data.ModelToWorldMatrix.Transpose();
			// m_ObjConstantBuffer->CopyData(0, data);
		}
	}
}

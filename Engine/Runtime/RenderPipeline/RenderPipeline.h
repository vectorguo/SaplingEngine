#pragma once

#include "RenderPipeline/CommandManager.h"
#include "RenderPipeline/GraphicsManager.h"
#include "RenderPipeline/RenderPass/RenderPass.h"
#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Dx12GraphicsManager;
	class RenderPass;
	
	class RenderPipeline final
	{
	public:
		RenderPipeline();
		~RenderPipeline();

		SINGLETON(RenderPipeline)

		/**
		 * \brief 开始初始化
		 */
		void BeginInitialize(HWND hWnd);

		/**
		 * \brief 结束初始化
		 */
		void EndInitialize(HWND hWnd);

		/**
		 * \brief 渲染
		 */
		void Render();
		
		/**
		 * \brief 销毁
		 */
		void Destroy();

		/**
		 * \brief 设置宽度和高度
		 */
		void OnWindowResize();
		
		/**
		 * \brief 添加RenderPass
		 * \param pRenderPass 渲染管线指针,Add之后由RenderPipeline管理每个Pass的生命周期
		 */
		void AddRenderPass(RenderPass* pRenderPass);

		/**
		 * \brief 删除RenderPass
		 * \param renderPassName RenderPass名称
		 */
		void RemoveRenderPass(const std::string& renderPassName);
		
	private:
		/**
		 * \brief 执行渲染前的准备工作
		 */
		void PreRender();

		/**
		 * \brief 执行渲染后的清理工作
		 */
		void PostRender();
		
		/**
		 * \brief 根据优先级对RenderPass进行排序
		 */
		void SortRenderPass();

		/**
		 * \brief 更新常量缓冲区
		 * \param pCamera 渲染使用的相机
		 * \param pActiveScene 当前活动场景
		 */
		void UpdateConstantBuffer(const Camera* pCamera, Scene* pActiveScene);
		
	private:
		/**
		 * \brief DX12命令管理器
		 */
		CommandManager* m_pCommandManager = nullptr;
		
		/**
		 * \brief DX12图形管理器
		 */
		GraphicsManager* m_pGraphicsManager = nullptr;

		/**
		 * \brief 屏幕宽度
		 */
		uint32_t m_ScreenWidth = 0;

		/**
		 * \brief 屏幕高度
		 */
		uint32_t m_ScreenHeight = 0;
		
		/**
		 * \brief 所有RenderPass
		 */
		std::vector<RenderPass*> m_RenderPasses{};
	};
}

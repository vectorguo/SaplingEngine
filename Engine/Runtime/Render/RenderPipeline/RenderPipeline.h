#pragma once

#include "SaplingEnginePch.h"
#include "Render/Graphics/DirectX12/Dx12CommandManager.h"
#include "Render/Graphics/DirectX12/Dx12CBufferManager.h"
#include "Render/Graphics/DirectX12/Dx12GraphicsManager.h"

namespace SaplingEngine
{
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
		 * \param width 屏幕宽度
		 * \param height 屏幕高度
		 */
		void OnSceneResize(uint32_t width, uint32_t height);
		
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
		
	private:
		/**
		 * \brief DX12命令管理器
		 */
		CommandManager* m_pCommandManager = nullptr;

		/**
		 * \brief 常量缓冲区管理器
		 */
		CBufferManager* m_pConstantBufferManager = nullptr;
		
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

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
		/**
		 * \brief 开始初始化
		 */
		static void BeginInitialize(HWND hWnd);

		/**
		 * \brief 结束初始化
		 */
		static void EndInitialize(HWND hWnd);

		/**
		 * \brief 渲染
		 */
		static void Render();
		
		/**
		 * \brief 销毁
		 */
		static void Destroy();

		/**
		 * \brief 设置宽度和高度
		 * \param width 屏幕宽度
		 * \param height 屏幕高度
		 */
		static void OnSceneResize(uint32_t width, uint32_t height);
		
		/**
		 * \brief 添加RenderPass
		 * \param pRenderPass 渲染管线指针,Add之后由RenderPipeline管理每个Pass的生命周期
		 */
		static void AddRenderPass(RenderPass* pRenderPass);

		/**
		 * \brief 删除RenderPass
		 * \param renderPassName RenderPass名称
		 */
		static void RemoveRenderPass(const std::string& renderPassName);
		
	private:
		/**
		 * \brief 执行渲染前的准备工作
		 */
		static void PreRender();

		/**
		 * \brief 执行渲染后的清理工作
		 */
		static void PostRender();
		
		/**
		 * \brief 根据优先级对RenderPass进行排序
		 */
		static void SortRenderPass();
		
	private:
		/**
		 * \brief 屏幕宽度
		 */
		static uint32_t screenWidth;

		/**
		 * \brief 屏幕高度
		 */
		static uint32_t screenHeight;
		
		/**
		 * \brief 所有RenderPass
		 */
		static std::vector<RenderPass*> renderPasses;
	};
}

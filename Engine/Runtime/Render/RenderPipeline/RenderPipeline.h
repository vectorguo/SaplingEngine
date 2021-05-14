#pragma once

#include "SaplingEnginePch.h"
#include "Render/Graphics/DirectX12/Dx12CommandManager.h"
#include "Render/Graphics/DirectX12/Dx12CBufferManager.h"
#include "Render/Graphics/DirectX12/Dx12GraphicsManager.h"
#include "Render/Renderer/Renderer.h"

namespace SaplingEngine
{
	class RenderPass;
	
	class RenderPipeline final
	{
	public:
		/**
		 * \brief	开始初始化
		 */
		static void BeginInitialize(HWND hWnd);

		/**
		 * \brief	结束初始化
		 */
		static void EndInitialize(HWND hWnd);

		/**
		 * \brief	渲染
		 */
		static void Render();
		
		/**
		 * \brief	销毁
		 */
		static void Destroy();

		/**
		 * \brief	设置宽度和高度
		 * \param	width		屏幕宽度
		 * \param	height		屏幕高度
		 */
		static void OnSceneResize(uint32_t width, uint32_t height);
		
		/**
		 * \brief	添加渲染项
		 * \param	pRenderer	renderer指针
		 * \param	shaderHashValue	Shader对应的HashValue
		 */
		static void AddRenderItem(Renderer* pRenderer, size_t shaderHashValue)
		{
			auto iter = renderItems.find(shaderHashValue);
			if (iter == renderItems.end())
			{
				std::vector<Renderer*> items;
				items.reserve(CBufferManager::ConstantBufferElementCount);
				items.emplace_back(pRenderer);
				renderItems.emplace(shaderHashValue, std::move(items));
			}
			else
			{
				iter->second.emplace_back(pRenderer);
			}
		}

		/**
		 * \brief	获取所有渲染项
		 * \return	所有渲染项(非const)
		 */
		static std::map<size_t, std::vector<Renderer*>>& GetRenderItems()
		{
			return renderItems;
		}

		/**
		 * \brief	删除渲染项
		 * \param	pRenderer	renderer指针
		 * \param	shaderHashValue	Shader对应的HashValue
		 */
		static void RemoveRenderItem(Renderer* pRenderer, size_t shaderHashValue)
		{
			auto iter1 = renderItems.find(shaderHashValue);
			if (iter1 != renderItems.end())
			{
				iter1->second.erase(std::find(iter1->second.begin(), iter1->second.end(), pRenderer));

				//归还常量缓冲区索引
				CBufferManager::PushCbvIndex(shaderHashValue, pRenderer->GetCbvIndex());
			}
		}

		/**
		 * \brief	添加RenderPass
		 * \param	pRenderPass		渲染管线指针,Add之后由RenderPipeline管理每个Pass的生命周期
		 */
		static void AddRenderPass(RenderPass* pRenderPass);

		/**
		 * \brief	删除RenderPass
		 * \param	renderPassName	RenderPass名称
		 */
		static void RemoveRenderPass(const std::string& renderPassName);

		/**
		 * \brief	根据优先级对RenderPass进行排序
		 */
		static void SortRenderPass();
		
	private:
		/**
		 * \brief	执行渲染前的准备工作
		 */
		static void PreRender();

		/**
		 * \brief	执行渲染后的清理工作
		 */
		static void PostRender();
	

		/**
		 * \brief	更新常量缓冲区数据
		 */
		static void UpdateCbvData(Camera* pCamera);
		
	private:
		/**
		 * \brief	屏幕宽度
		 */
		static uint32_t screenWidth;

		/**
		 * \brief	屏幕高度
		 */
		static uint32_t screenHeight;

		/**
		 * \brief	渲染项列表
		 */
		static std::map<size_t, std::vector<Renderer*>> renderItems;
		
		/**
		 * \brief	所有RenderPass
		 */
		static std::vector<RenderPass*> renderPasses;
	};
}

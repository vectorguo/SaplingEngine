#pragma once

#include "Render/Graphics/DirectX12/Dx12BufferManager.h"
#include "Render/Graphics/DirectX12/Dx12CommandManager.h"
#include "Render/Graphics/DirectX12/Dx12DescriptorManager.h"
#include "Render/Graphics/DirectX12/Dx12GraphicsManager.h"
#include "Render/Renderer/Renderer.h"
#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class RenderPass;
	class RenderOpaquePass;
	class ShadowPass;
	
	class RenderPipeline final
	{
	public:
		/**
		 * \brief	渲染项列表
		 */
		class RenderItemList
		{
		public:
			/**
			 * \brief	添加渲染项
			 * \param	pRender		Render指针
			 */
			void AddRenderItem(Renderer* pRender)
			{
				auto index = 0;
				for (; index < m_DescriptorHeaps.size(); ++index)
				{					
					if (m_Renderers[index].size() < Dx12DescriptorManager::ObjectCbvDescriptorCount)
					{
						break;
					}
				}

				if (index == m_DescriptorHeaps.size())
				{
					m_DescriptorHeaps.push_back(Dx12DescriptorManager::GetObjectCbvDescriptorHeap());
					m_Renderers.emplace_back();
				}

				auto& uploadBuffer = m_DescriptorHeaps[index]->UploadBuffer;
				auto& renders = m_Renderers[index];
				pRender->m_CbvHeapIndex = index;
				pRender->m_CbvIndex = static_cast<uint32_t>(renders.size());
				pRender->m_CommonCbAddress = uploadBuffer.GetGpuVirtualAddress(pRender->m_CbvIndex * Dx12DescriptorManager::ObjectCommonCbSize);
				pRender->m_SpecialCbAddress = uploadBuffer.GetGpuVirtualAddress(pRender->m_CbvIndex * Dx12DescriptorManager::ObjectSpecialCbSize + Dx12DescriptorManager::TotalObjectCommonCbSize);
				renders.push_back(pRender);
			}

			/**
			 * \brief	删除渲染项
			 * \param	pRender		Render指针
			 */
			void RemoveRenderItem(Renderer* pRender)
			{

			}

		private:
			/**
			 * \brief	描述符堆指针列表
			 */
			std::vector<Dx12DescriptorManager::Dx12DescriptorHeap*> m_DescriptorHeaps;

			/**
			 * \brief	渲染项列表
			 */
			std::vector<std::vector<Renderer*>> m_Renderers;
		};

	public:
		/**
		 * \brief	初始化
		 */
		static void Initialize(HWND hWnd);

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
				items.reserve(BufferManager::ConstantBufferElementCount);
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
				BufferManager::PushCbvIndex(pRenderer->GetCbvIndex());
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

		/**
		 * \brief	获取ShadowPass
		 */
		static const ShadowPass* GetShadowPass()
		{
			return shadowPassPtr;
		}
		
	private:
		/**
		 * \brief	开始渲染
		 * \param	pCamera			相机指针
		 */
		static void BeginCameraRendering(const Camera* pCamera);

		/**
		 * \brief	渲染单相机
		 * \param	pCamera			相机指针
		 */
		static void RenderSingleCamera(const Camera* pCamera);

		/**
		 * \brief	结束渲染
		 * \param	pCamera			相机指针
		 */
		static void EndCameraRendering(const Camera* pCamera);

		/**
		 * \brief	执行渲染前的准备工作
		 */
		static void PreRender();

		/**
		 * \brief	执行渲染后的清理工作
		 */
		static void PostRender();
	
		/**
		 * \brief	更新物体常量缓冲区数据
		 */
		static void UpdateObjectCbvData();

		/**
		 * \brief	更新Pass常量缓冲区数据
		 */
		static void UpdatePassCbvData(Camera* pCamera);
		
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
		static std::map<size_t, RenderItemList> renderItemsMap;
		
		/**
		 * \brief	所有RenderPass
		 */
		static std::vector<RenderPass*> renderPasses;

		/**
		 * \brief	Render Opaque Pass
		 */
		static RenderOpaquePass* opaquePassPtr;

		/**
		 * \brief	Shadow Pass
		 */
		static ShadowPass* shadowPassPtr;
	};
}

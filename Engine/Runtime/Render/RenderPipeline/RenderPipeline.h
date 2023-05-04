#pragma once

#include "Runtime/SaplingEnginePch.h"
#include "Runtime/Render/Graphics/DirectX12/Dx12CommandManager.h"
#include "Runtime/Render/Graphics/DirectX12/Dx12DescriptorManager.h"
#include "Runtime/Render/Graphics/DirectX12/Dx12GraphicsManager.h"
#include "Runtime/Render/Render/Renderer.h"

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
		class RenderItemContainer
		{
			struct ContainerElement
			{
				void Initialize()
				{
					Dx12DescriptorManager::GetObjectCbvDescriptorHeap(DescriptorHeapCptr, UploadBufferPtr);
				}

				void Destroy()
				{
					Dx12DescriptorManager::ReturnObjectCbvDescriptorHeap(DescriptorHeapCptr, UploadBufferPtr);
				}
				
				bool isFull()
				{
					return Renderers.size() >= Dx12DescriptorManager::ObjectCbvDescriptorCount;
				}

				ComPtr<ID3D12DescriptorHeap> DescriptorHeapCptr;
				Dx12UploadBuffer* UploadBufferPtr = nullptr;
				std::vector<Renderer*> Renderers;
			};

		public:
			/**
			 * \brief	添加渲染项
			 * \param	pRender		Render指针
			 */
			void AddRenderItem(Renderer* pRender)
			{
				auto index = 0;
				for (; index < elements.size(); ++index)
				{
					if (!elements[index].isFull())
					{
						break;
					}
				}

				if (index == elements.size())
				{
					elements.emplace_back();
					elements.rbegin()->Initialize();
				}

				auto& element = elements[index];
				pRender->m_CbvHeapIndex = index;
				pRender->m_CbvIndex = static_cast<uint32_t>(element.Renderers.size());
				pRender->m_CommonCbAddress = element.UploadBufferPtr->GetGpuVirtualAddress(pRender->m_CbvIndex * Dx12DescriptorManager::ObjectCommonCbSize);
				pRender->m_SpecialCbAddress = element.UploadBufferPtr->GetGpuVirtualAddress(pRender->m_CbvIndex * Dx12DescriptorManager::ObjectSpecialCbSize + Dx12DescriptorManager::TotalObjectCommonCbSize);
				element.Renderers.push_back(pRender);
			}

			/**
			 * \brief	删除渲染项
			 * \param	pRender		Render指针
			 */
			void RemoveRenderItem(Renderer* pRender)
			{
				//修改最后位置的Render的参数与要删除的Render的参数一致
				auto& lastElement = *elements.rbegin();
				auto* pLastRender = *lastElement.Renderers.rbegin();
				pLastRender->m_CbvHeapIndex = pRender->m_CbvHeapIndex;
				pLastRender->m_CbvIndex = pRender->m_CbvIndex;
				pLastRender->m_CommonCbAddress = pRender->m_CommonCbAddress;
				pLastRender->m_SpecialCbAddress = pRender->m_SpecialCbAddress;

				//将最后位置上的Render挪到要被删除的Render的位置上
				elements[pRender->m_CbvHeapIndex].Renderers[pRender->m_CbvIndex] = pLastRender;

				//重置被删除的渲染项的数据
				pRender->m_CbvHeapIndex = -1;
				pRender->m_CbvIndex = -1;

				//删除最后位置上的Render
				lastElement.Renderers.pop_back();
				if (lastElement.Renderers.empty())
				{
					lastElement.Destroy();
					elements.pop_back();
				}
			}

			/**
			 * \brief	上传填充数据
			 */
			void UploadRenderData()
			{
				size_t specialDataSize;
				for (auto iter1 = elements.begin(); iter1 != elements.end(); ++iter1)
				{
					auto* pUploadBuffer = iter1->UploadBufferPtr;
					for (auto iter2 = iter1->Renderers.begin(); iter2 != iter1->Renderers.end(); ++iter2)
					{
						auto* pRender = *iter2;

						//填充物体通用常量缓冲区数据
						const auto* pCommonData = CommonOcbData::FillOcbData(pRender->GetTransform());
						pUploadBuffer->CopyData(pCommonData, CommonOcbData::DataSize, pRender->m_CbvIndex * Dx12DescriptorManager::ObjectCommonCbSize);

						//填充物体特殊常量缓冲区数据
						const auto* pSpecialData = pRender->FillSpecialOcbData(specialDataSize, pRender->GetMaterial());
						pUploadBuffer->CopyData(pSpecialData, (uint32_t)specialDataSize, pRender->m_CbvIndex * Dx12DescriptorManager::ObjectSpecialCbSize + Dx12DescriptorManager::TotalObjectCommonCbSize);
					}
				}
			}

		public:
			std::vector<ContainerElement> elements;
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
			renderItemsMap[shaderHashValue].AddRenderItem(pRenderer);
		}

		/**
		 * \brief	获取所有渲染项
		 * \return	所有渲染项(非const)
		 */
		static std::map<size_t, RenderItemContainer>& GetRenderItems()
		{
			return renderItemsMap;
		}

		/**
		 * \brief	删除渲染项
		 * \param	pRenderer	renderer指针
		 * \param	shaderHashValue	Shader对应的HashValue
		 */
		static void RemoveRenderItem(Renderer* pRenderer, size_t shaderHashValue)
		{
			renderItemsMap[shaderHashValue].RemoveRenderItem(pRenderer);
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
		 * \brief	上传物体常量缓冲区数据
		 */
		static void UploadObjectCbvData();

		/**
		 * \brief	上传Pass常量缓冲区数据
		 */
		static void UploadPassCbvData(Camera* pCamera);
		
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
		static std::map<size_t, RenderItemContainer> renderItemsMap;
		
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

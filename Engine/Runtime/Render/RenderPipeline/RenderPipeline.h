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
		 * \brief	��Ⱦ���б�
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
			 * \brief	�����Ⱦ��
			 * \param	pRender		Renderָ��
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
			 * \brief	ɾ����Ⱦ��
			 * \param	pRender		Renderָ��
			 */
			void RemoveRenderItem(Renderer* pRender)
			{
				//�޸����λ�õ�Render�Ĳ�����Ҫɾ����Render�Ĳ���һ��
				auto& lastElement = *elements.rbegin();
				auto* pLastRender = *lastElement.Renderers.rbegin();
				pLastRender->m_CbvHeapIndex = pRender->m_CbvHeapIndex;
				pLastRender->m_CbvIndex = pRender->m_CbvIndex;
				pLastRender->m_CommonCbAddress = pRender->m_CommonCbAddress;
				pLastRender->m_SpecialCbAddress = pRender->m_SpecialCbAddress;

				//�����λ���ϵ�RenderŲ��Ҫ��ɾ����Render��λ����
				elements[pRender->m_CbvHeapIndex].Renderers[pRender->m_CbvIndex] = pLastRender;

				//���ñ�ɾ������Ⱦ�������
				pRender->m_CbvHeapIndex = -1;
				pRender->m_CbvIndex = -1;

				//ɾ�����λ���ϵ�Render
				lastElement.Renderers.pop_back();
				if (lastElement.Renderers.empty())
				{
					lastElement.Destroy();
					elements.pop_back();
				}
			}

			/**
			 * \brief	�ϴ��������
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

						//�������ͨ�ó�������������
						const auto* pCommonData = CommonOcbData::FillOcbData(pRender->GetTransform());
						pUploadBuffer->CopyData(pCommonData, CommonOcbData::DataSize, pRender->m_CbvIndex * Dx12DescriptorManager::ObjectCommonCbSize);

						//����������ⳣ������������
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
		 * \brief	��ʼ��
		 */
		static void Initialize(HWND hWnd);

		/**
		 * \brief	��Ⱦ
		 */
		static void Render();
		
		/**
		 * \brief	����
		 */
		static void Destroy();

		/**
		 * \brief	���ÿ�Ⱥ͸߶�
		 * \param	width		��Ļ���
		 * \param	height		��Ļ�߶�
		 */
		static void OnSceneResize(uint32_t width, uint32_t height);
		
		/**
		 * \brief	�����Ⱦ��
		 * \param	pRenderer	rendererָ��
		 * \param	shaderHashValue	Shader��Ӧ��HashValue
		 */
		static void AddRenderItem(Renderer* pRenderer, size_t shaderHashValue)
		{
			renderItemsMap[shaderHashValue].AddRenderItem(pRenderer);
		}

		/**
		 * \brief	��ȡ������Ⱦ��
		 * \return	������Ⱦ��(��const)
		 */
		static std::map<size_t, RenderItemContainer>& GetRenderItems()
		{
			return renderItemsMap;
		}

		/**
		 * \brief	ɾ����Ⱦ��
		 * \param	pRenderer	rendererָ��
		 * \param	shaderHashValue	Shader��Ӧ��HashValue
		 */
		static void RemoveRenderItem(Renderer* pRenderer, size_t shaderHashValue)
		{
			renderItemsMap[shaderHashValue].RemoveRenderItem(pRenderer);
		}

		/**
		 * \brief	���RenderPass
		 * \param	pRenderPass		��Ⱦ����ָ��,Add֮����RenderPipeline����ÿ��Pass����������
		 */
		static void AddRenderPass(RenderPass* pRenderPass);

		/**
		 * \brief	ɾ��RenderPass
		 * \param	renderPassName	RenderPass����
		 */
		static void RemoveRenderPass(const std::string& renderPassName);

		/**
		 * \brief	�������ȼ���RenderPass��������
		 */
		static void SortRenderPass();

		/**
		 * \brief	��ȡShadowPass
		 */
		static const ShadowPass* GetShadowPass()
		{
			return shadowPassPtr;
		}
		
	private:
		/**
		 * \brief	��ʼ��Ⱦ
		 * \param	pCamera			���ָ��
		 */
		static void BeginCameraRendering(const Camera* pCamera);

		/**
		 * \brief	��Ⱦ�����
		 * \param	pCamera			���ָ��
		 */
		static void RenderSingleCamera(const Camera* pCamera);

		/**
		 * \brief	������Ⱦ
		 * \param	pCamera			���ָ��
		 */
		static void EndCameraRendering(const Camera* pCamera);

		/**
		 * \brief	ִ����Ⱦǰ��׼������
		 */
		static void PreRender();

		/**
		 * \brief	ִ����Ⱦ���������
		 */
		static void PostRender();
	
		/**
		 * \brief	�ϴ����峣������������
		 */
		static void UploadObjectCbvData();

		/**
		 * \brief	�ϴ�Pass��������������
		 */
		static void UploadPassCbvData(Camera* pCamera);
		
	private:
		/**
		 * \brief	��Ļ���
		 */
		static uint32_t screenWidth;

		/**
		 * \brief	��Ļ�߶�
		 */
		static uint32_t screenHeight;

		/**
		 * \brief	��Ⱦ���б�
		 */
		static std::map<size_t, RenderItemContainer> renderItemsMap;
		
		/**
		 * \brief	����RenderPass
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

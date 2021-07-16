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
		 * \brief	��Ⱦ���б�
		 */
		class RenderItemList
		{
		public:
			/**
			 * \brief	�����Ⱦ��
			 * \param	pRender		Renderָ��
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
			 * \brief	ɾ����Ⱦ��
			 * \param	pRender		Renderָ��
			 */
			void RemoveRenderItem(Renderer* pRender)
			{

			}

		private:
			/**
			 * \brief	��������ָ���б�
			 */
			std::vector<Dx12DescriptorManager::Dx12DescriptorHeap*> m_DescriptorHeaps;

			/**
			 * \brief	��Ⱦ���б�
			 */
			std::vector<std::vector<Renderer*>> m_Renderers;
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
		 * \brief	��ȡ������Ⱦ��
		 * \return	������Ⱦ��(��const)
		 */
		static std::map<size_t, std::vector<Renderer*>>& GetRenderItems()
		{
			return renderItems;
		}

		/**
		 * \brief	ɾ����Ⱦ��
		 * \param	pRenderer	rendererָ��
		 * \param	shaderHashValue	Shader��Ӧ��HashValue
		 */
		static void RemoveRenderItem(Renderer* pRenderer, size_t shaderHashValue)
		{
			auto iter1 = renderItems.find(shaderHashValue);
			if (iter1 != renderItems.end())
			{
				iter1->second.erase(std::find(iter1->second.begin(), iter1->second.end(), pRenderer));

				//�黹��������������
				BufferManager::PushCbvIndex(pRenderer->GetCbvIndex());
			}
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
		 * \brief	�������峣������������
		 */
		static void UpdateObjectCbvData();

		/**
		 * \brief	����Pass��������������
		 */
		static void UpdatePassCbvData(Camera* pCamera);
		
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
		static std::map<size_t, std::vector<Renderer*>> renderItems;
		static std::map<size_t, RenderItemList> renderItemsMap;
		
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

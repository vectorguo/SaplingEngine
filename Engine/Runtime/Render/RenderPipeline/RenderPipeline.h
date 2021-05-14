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
		 * \brief	��ʼ��ʼ��
		 */
		static void BeginInitialize(HWND hWnd);

		/**
		 * \brief	������ʼ��
		 */
		static void EndInitialize(HWND hWnd);

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
				CBufferManager::PushCbvIndex(shaderHashValue, pRenderer->GetCbvIndex());
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
		
	private:
		/**
		 * \brief	ִ����Ⱦǰ��׼������
		 */
		static void PreRender();

		/**
		 * \brief	ִ����Ⱦ���������
		 */
		static void PostRender();
	

		/**
		 * \brief	���³�������������
		 */
		static void UpdateCbvData(Camera* pCamera);
		
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
		
		/**
		 * \brief	����RenderPass
		 */
		static std::vector<RenderPass*> renderPasses;
	};
}

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
		 * \brief ��ʼ��ʼ��
		 */
		static void BeginInitialize(HWND hWnd);

		/**
		 * \brief ������ʼ��
		 */
		static void EndInitialize(HWND hWnd);

		/**
		 * \brief ��Ⱦ
		 */
		static void Render();
		
		/**
		 * \brief ����
		 */
		static void Destroy();

		/**
		 * \brief ���ÿ�Ⱥ͸߶�
		 * \param width ��Ļ���
		 * \param height ��Ļ�߶�
		 */
		static void OnSceneResize(uint32_t width, uint32_t height);
		
		/**
		 * \brief ���RenderPass
		 * \param pRenderPass ��Ⱦ����ָ��,Add֮����RenderPipeline����ÿ��Pass����������
		 */
		static void AddRenderPass(RenderPass* pRenderPass);

		/**
		 * \brief ɾ��RenderPass
		 * \param renderPassName RenderPass����
		 */
		static void RemoveRenderPass(const std::string& renderPassName);
		
	private:
		/**
		 * \brief ִ����Ⱦǰ��׼������
		 */
		static void PreRender();

		/**
		 * \brief ִ����Ⱦ���������
		 */
		static void PostRender();
		
		/**
		 * \brief �������ȼ���RenderPass��������
		 */
		static void SortRenderPass();
		
	private:
		/**
		 * \brief ��Ļ���
		 */
		static uint32_t screenWidth;

		/**
		 * \brief ��Ļ�߶�
		 */
		static uint32_t screenHeight;
		
		/**
		 * \brief ����RenderPass
		 */
		static std::vector<RenderPass*> renderPasses;
	};
}

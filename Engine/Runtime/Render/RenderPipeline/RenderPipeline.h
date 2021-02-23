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
		 * \brief ��ʼ��ʼ��
		 */
		void BeginInitialize(HWND hWnd);

		/**
		 * \brief ������ʼ��
		 */
		void EndInitialize(HWND hWnd);

		/**
		 * \brief ��Ⱦ
		 */
		void Render();
		
		/**
		 * \brief ����
		 */
		void Destroy();

		/**
		 * \brief ���ÿ�Ⱥ͸߶�
		 * \param width ��Ļ���
		 * \param height ��Ļ�߶�
		 */
		void OnSceneResize(uint32_t width, uint32_t height);
		
		/**
		 * \brief ���RenderPass
		 * \param pRenderPass ��Ⱦ����ָ��,Add֮����RenderPipeline����ÿ��Pass����������
		 */
		void AddRenderPass(RenderPass* pRenderPass);

		/**
		 * \brief ɾ��RenderPass
		 * \param renderPassName RenderPass����
		 */
		void RemoveRenderPass(const std::string& renderPassName);
		
	private:
		/**
		 * \brief ִ����Ⱦǰ��׼������
		 */
		void PreRender();

		/**
		 * \brief ִ����Ⱦ���������
		 */
		void PostRender();
		
		/**
		 * \brief �������ȼ���RenderPass��������
		 */
		void SortRenderPass();
		
	private:
		/**
		 * \brief DX12���������
		 */
		CommandManager* m_pCommandManager = nullptr;

		/**
		 * \brief ����������������
		 */
		CBufferManager* m_pConstantBufferManager = nullptr;
		
		/**
		 * \brief DX12ͼ�ι�����
		 */
		GraphicsManager* m_pGraphicsManager = nullptr;

		/**
		 * \brief ��Ļ���
		 */
		uint32_t m_ScreenWidth = 0;

		/**
		 * \brief ��Ļ�߶�
		 */
		uint32_t m_ScreenHeight = 0;
		
		/**
		 * \brief ����RenderPass
		 */
		std::vector<RenderPass*> m_RenderPasses{};
	};
}

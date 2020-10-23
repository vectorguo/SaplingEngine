#pragma once

#include "GraphicsLibrary/DirectX12/Dx12CommandManager.h"
#include "GraphicsLibrary/DirectX12/Dx12GraphicsManager.h"
#include "RenderPass/RenderPass.h"
#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Dx12GraphicsManager;
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
		void BeginInitialize();

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
		 */
		void Resize();
		
		/**
		 * \brief ���RenderPass
		 * \param renderPassPtr RenderPass��unique_ptr
		 */
		void AddRenderPass(std::unique_ptr<RenderPass>&& renderPassPtr);

		/**
		 * \brief ɾ��RenderPass
		 * \param renderPassName RenderPass����
		 */
		void RemoveRenderPass(const std::string& renderPassName);

		/**
		 * \brief ��ȡGraphics Manager
		 * \return Graphics Manager
		 */
		Dx12GraphicsManager& GetGraphicsManager()
		{
			return m_GraphicsManager;
		}
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
		Dx12CommandManager m_CommandManager;
		
		/**
		 * \brief DX12ͼ�ι�����
		 */
		Dx12GraphicsManager m_GraphicsManager;

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
		std::vector<std::unique_ptr<RenderPass>> m_RenderPasses{};
	};
}

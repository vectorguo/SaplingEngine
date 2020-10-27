#pragma once

#include "RenderPipeline/CommandManager.h"
#include "RenderPipeline/GraphicsManager.h"
#include "RenderPipeline/RenderPass/RenderPass.h"
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
		 */
		void OnWindowResize();
		
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

		/**
		 * \brief ���³���������
		 * \param pCamera ��Ⱦʹ�õ����
		 * \param pActiveScene ��ǰ�����
		 */
		void UpdateConstantBuffer(const Camera* pCamera, Scene* pActiveScene);
		
	private:
		/**
		 * \brief DX12���������
		 */
		CommandManager* m_pCommandManager = nullptr;
		
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

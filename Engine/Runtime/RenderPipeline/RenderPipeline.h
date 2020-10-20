#pragma once

#include "SaplingEnginePch.h"
#include "RenderPass/RenderPass.h"

namespace SaplingEngine
{
	class RenderPass;
	
	class RenderPipeline final
	{
	public:
		RenderPipeline() = default;
		~RenderPipeline();

		SINGLETON(RenderPipeline)

		/**
		 * \brief ��ʼ��ʼ��
		 */
		void BeginInitialize(HWND hWnd);

		/**
		 * \brief ������ʼ��
		 */
		void EndInitialize();

		/**
		 * \brief ����
		 */
		void Destroy();
		
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
		
	private:
		/**
		 * \brief �������ȼ���RenderPass��������
		 */
		void SortRenderPass();
		
	private:
		std::vector<std::unique_ptr<RenderPass>> m_RenderPasses;
	};
}

#include "RenderPipeline.h"

#include "Graphics/DirectX12/D3D12GraphicsManager.h"

namespace SaplingEngine
{
	RenderPipeline::~RenderPipeline() = default;

	/**
	 * \brief ��ʼ��ʼ��
	 */
	void RenderPipeline::BeginInitialize(HWND hWnd)
	{
		D3D12GraphicsManager::Instance()->BeginInitialize(hWnd);
	}

	/**
	 * \brief ������ʼ��
	 */
	void RenderPipeline::EndInitialize()
	{
		D3D12GraphicsManager::Instance()->EndInitialize();
	}

	/**
	 * \brief ����
	 */
	void RenderPipeline::Destroy()
	{
		D3D12GraphicsManager::Instance()->Destroy();
	}

	/**
	 * \brief ���RenderPass
	 * \param renderPassPtr RenderPass��unique_ptr
	 */
	void RenderPipeline::AddRenderPass(std::unique_ptr<RenderPass>&& renderPassPtr)
	{
		const auto iter = std::find_if(m_RenderPasses.begin(), m_RenderPasses.end(), [&renderPassPtr](const std::unique_ptr<RenderPass>& rp)
			{
				return rp->GetName() == renderPassPtr->GetName();
			});
		if (iter == m_RenderPasses.end())
		{
			m_RenderPasses.push_back(std::move(renderPassPtr));
		}
	}
	
	/**
	 * \brief ɾ��RenderPass
	 * \param renderPassName RenderPass����
	 */
	void RenderPipeline::RemoveRenderPass(const std::string& renderPassName)
	{
		const auto iter = std::find_if(m_RenderPasses.begin(), m_RenderPasses.end(), [&renderPassName](const std::unique_ptr<RenderPass>& rp)
			{
				return rp->GetName() == renderPassName;
			});
		if (iter != m_RenderPasses.end())
		{
			m_RenderPasses.erase(iter);
		}
	}

	/**
	 * \brief �������ȼ���RenderPass��������
	 */
	void RenderPipeline::SortRenderPass()
	{
		std::sort(m_RenderPasses.begin(), m_RenderPasses.end(), [](const std::unique_ptr<RenderPass>& rp1, const std::unique_ptr<RenderPass>& rp2)
			{
				return rp1->GetPriority() < rp2->GetPriority();
			});
	}
}

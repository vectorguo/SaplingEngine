#include "RenderOpaquePass.h"

#include "RenderPipeline/CommandManager.h"
#include "RenderPipeline/Renderer/MeshRenderer.h"
#include "Scene/Scene.h"

namespace SaplingEngine
{
	/**
	 * \brief ��Ⱦ
	 * \param pCamera ��Ⱦʹ�õ����
	 * \param pActiveScene ����Ⱦ����
	 */
	void RenderOpaquePass::Render(const Camera* pCamera, const Scene* pActiveScene)
	{
		auto* pCommandManager = CommandManager::Instance();
		pCommandManager->ClearRenderTargets(true, true, m_BackgroundColor);
		pCommandManager->SetRootSignature();

		const auto& renderItems = pActiveScene->GetRenderItems();
		for (auto iter = renderItems.begin(); iter != renderItems.end(); ++iter)
		{
			pCommandManager->DrawIndexedInstanced(*iter);
		}
	}
}

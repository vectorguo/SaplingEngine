#include "RenderOpaquePass.h"

#include "RenderPipeline/CommandManager.h"
#include "RenderPipeline/Renderer/MeshRenderer.h"
#include "Scene/Scene.h"

namespace SaplingEngine
{
	/**
	 * \brief ��Ⱦ
	 * \param pCamera ��Ⱦʹ�õ����
	 * \param pScene ����Ⱦ����
	 */
	void RenderOpaquePass::Render(const Camera* pCamera, const Scene* pScene)
	{
		auto* pCommandManager = CommandManager::Instance();
		pCommandManager->ClearRenderTargets(true, true, m_BackgroundColor);
		pCommandManager->SetRootSignature();

		const auto& objects = pScene->GetGameObjects();
		for (auto iter = objects.begin(); iter != objects.end(); ++iter)
		{
			const auto pMeshRenderer = (*iter)->GetComponent<MeshRenderer>();
			if (pMeshRenderer != nullptr)
			{
				pCommandManager->DrawIndexedInstanced(pMeshRenderer->GetMesh(), pMeshRenderer->GetMaterial());
			}
		}
	}
}

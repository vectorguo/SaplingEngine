#include "RenderOpaquePass.h"

#include "RenderPipeline/CommandManager.h"
#include "RenderPipeline/Renderer/MeshRenderer.h"
#include "Scene/Scene.h"

namespace SaplingEngine
{
	/**
	 * \brief 渲染
	 * \param pCamera 渲染使用的相机
	 * \param pActiveScene 被渲染场景
	 */
	void RenderOpaquePass::Render(const Camera* pCamera, const Scene* pActiveScene)
	{
		auto* pCommandManager = CommandManager::Instance();
		pCommandManager->ClearRenderTargets(true, true, m_BackgroundColor);
		pCommandManager->SetRootSignature();

		const auto& objects = pActiveScene->GetGameObjects();
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

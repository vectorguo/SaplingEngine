#include "Renderer.h"

#include "GameObject/GameObject.h"
#include "Graphics/Material.h"
#include "Graphics/Mesh.h"
#include "RenderPipeline/GraphicsManager.h"
#include "Scene/Scene.h"

namespace SaplingEngine
{
	Renderer::Renderer()
	{
		m_ConstantBufferIndex = GraphicsManager::Instance()->GetObjectConstantBufferIndex();
	}

	Renderer::~Renderer()
	{
		delete m_pMaterial;
		delete m_pMesh;

		if (m_ConstantBufferIndex >= 0)
		{
			GraphicsManager::Instance()->ReturnObjectConstantBufferIndex(m_ConstantBufferIndex);
		}
	}

	void Renderer::Start()
	{
		if (m_ConstantBufferIndex >= 0)
		{
			auto* pActiveScene = m_GameObjectSptr->GetScene();
			pActiveScene->AddRenderItem(this);
		}
	}

	void Renderer::OnDestroy()
	{
		if (m_ConstantBufferIndex >= 0)
		{
			auto* pActiveScene = m_GameObjectSptr->GetScene();
			pActiveScene->RemoveRenderItem(this);
		}
	}
}

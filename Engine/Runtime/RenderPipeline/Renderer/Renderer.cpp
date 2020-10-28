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
		
		GraphicsManager::Instance()->ReturnObjectConstantBufferIndex(m_ConstantBufferIndex);
	}

	void Renderer::Start()
	{
		auto* pActiveScene = m_pGameObject->GetScene();
		pActiveScene->AddRenderItem(this);
	}

	void Renderer::OnDestroy()
	{
		auto* pActiveScene = m_pGameObject->GetScene();
		pActiveScene->RemoveRenderItem(this);
	}
}

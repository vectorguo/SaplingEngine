#include "Renderer.h"

#include "GameObject/GameObject.h"
#include "Graphics/ConstantBufferData.h"
#include "Graphics/Material.h"
#include "Graphics/Mesh.h"
#include "RenderPipeline/GraphicsManager.h"
#include "Scene/Scene.h"

namespace SaplingEngine
{
	Renderer::Renderer()
	{
		m_OcbIndex = GraphicsManager::Instance()->PopObjectCbIndex();
	}

	Renderer::~Renderer()
	{
		delete m_pMaterial;
		delete m_pMesh;
		delete m_pSpecialOcbData;

		if (m_OcbIndex >= 0)
		{
			GraphicsManager::Instance()->PushObjectCbIndex(m_OcbIndex);
		}
	}	

	void Renderer::Start()
	{
		if (m_OcbIndex >= 0)
		{
			auto* pActiveScene = m_GameObjectSptr->GetScene();
			pActiveScene->AddRenderItem(this);
		}
	}

	void Renderer::OnDestroy()
	{
		if (m_OcbIndex >= 0)
		{
			auto* pActiveScene = m_GameObjectSptr->GetScene();
			pActiveScene->RemoveRenderItem(this);
		}
	}
}

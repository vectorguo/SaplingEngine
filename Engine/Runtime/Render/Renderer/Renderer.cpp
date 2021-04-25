#include "Renderer.h"

#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

namespace SaplingEngine
{
	Renderer::Renderer()
	{
		m_OcbIndex = CBufferManager::PopObjectCbIndex();
	}

	Renderer::~Renderer()
	{
		delete m_pMaterial;
		delete m_pMesh;
		delete m_pSpecialOcbData;

		//归还常量缓冲区索引
		CBufferManager::PushObjectCbIndex(m_OcbIndex);
	}	

	void Renderer::Start()
	{
		auto* pActiveScene = m_GameObjectSptr->GetScene();
		pActiveScene->AddRenderItem(this);
	}

	void Renderer::OnDestroy()
	{
		auto* pActiveScene = m_GameObjectSptr->GetScene();
		pActiveScene->RemoveRenderItem(this);
	}
}

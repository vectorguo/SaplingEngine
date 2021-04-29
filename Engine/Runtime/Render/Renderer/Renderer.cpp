#include "Renderer.h"

#include "GameObject/GameObject.h"
#include "Render/Graphics/MeshFactory.h"
#include "Render/RenderPipeline/RenderPipeline.h"
#include "Scene/Scene.h"

namespace SaplingEngine
{
	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
		delete m_pMaterial;

		//É¾³ýMesh
		MeshFactory::DestroyMesh(m_pMesh);
	}	

	void Renderer::Start()
	{
		const auto& shaderHashValue = m_pMaterial->GetShaderHashValue();
		m_CbvIndex = CBufferManager::PopCbvIndex(shaderHashValue, m_CommonCbvDescriptor, m_SpecialCbvDescriptor);
		RenderPipeline::AddRenderItem(this, shaderHashValue);
	}

	void Renderer::OnDestroy()
	{
		RenderPipeline::RemoveRenderItem(this, m_pMaterial->GetShaderHashValue());
	}
}

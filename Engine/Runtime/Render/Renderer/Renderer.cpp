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
		delete m_pSpecialOcbData;

		//É¾³ýMesh
		MeshFactory::DestroyMesh(m_pMesh);
	}	

	void Renderer::Start()
	{
		const auto& shaderName = m_pMaterial->GetShaderName();
		m_CbvIndex = CBufferManager::PopCbvIndex(shaderName, m_CommonCbvDescriptor, m_SpecialCbvDescriptor);
		RenderPipeline::AddRenderItem(this, shaderName);
	}

	void Renderer::OnDestroy()
	{
		RenderPipeline::RemoveRenderItem(this, m_pMaterial->GetShaderName());
	}
}

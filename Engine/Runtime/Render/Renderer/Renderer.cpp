#include "Renderer.h"

#include "GameObject/GameObject.h"
#include "Render/Graphics/MeshFactory.h"
#include "Render/RenderPipeline/RenderPipeline.h"
#include "Scene/Scene.h"

namespace SaplingEngine
{
	Renderer::Renderer() : Component(ComponentType_Renderer)
	{
	}

	Renderer::~Renderer()
	{
		//É¾³ýMesh
		MeshFactory::DestroyMesh(std::move(m_MeshSptr));
	}	

	void Renderer::Start()
	{
		const auto& shaderHashValue = m_MaterialSptr->GetShaderHashValue();
		m_CbvIndex = CBufferManager::PopCbvIndex(shaderHashValue, m_CommonCbvDescriptor, m_SpecialCbvDescriptor);
		RenderPipeline::AddRenderItem(this, shaderHashValue);
	}

	void Renderer::OnDestroy()
	{
		RenderPipeline::RemoveRenderItem(this, m_MaterialSptr->GetShaderHashValue());
	}
}

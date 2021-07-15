#include "Renderer.h"

#include "GameObject/GameObject.h"
#include "Render/Graphics/MeshManager.h"
#include "Render/RenderPipeline/RenderPipeline.h"
#include "Scene/Scene.h"

namespace SaplingEngine
{
	Renderer::Renderer() : Component(ComponentType_Renderer)
	{
		m_RunInEditorMode = true;
	}

	Renderer::~Renderer()
	{
		//É¾³ýMesh
		MeshManager::DestroyMesh(std::move(m_MeshSptr));
	}	

	void Renderer::Start()
	{
		const auto& shaderHashValue = m_MaterialSptr->GetShaderHashValue();
		m_CbvIndex = BufferManager::PopCbvIndex(m_CommonCbAddress, m_SpecialCbAddress);
		RenderPipeline::AddRenderItem(this, shaderHashValue);
	}

	void Renderer::OnEnable()
	{
	}

	void Renderer::OnDisable()
	{
	}

	void Renderer::OnDestroy()
	{
		RenderPipeline::RemoveRenderItem(this, m_MaterialSptr->GetShaderHashValue());
	}
}

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
		//删除Mesh
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
		//添加到渲染管线中
	}

	void Renderer::OnDisable()
	{
		//从渲染管线中移除
	}

	void Renderer::OnDestroy()
	{
		RenderPipeline::RemoveRenderItem(this, m_MaterialSptr->GetShaderHashValue());
	}
}

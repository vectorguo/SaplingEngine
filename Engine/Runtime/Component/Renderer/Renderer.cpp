#include "Renderer.h"

#include "Runtime/GameObject/GameObject.h"
#include "Runtime/Render/Graphics/Material/Material.h"
#include "Runtime/Render/Graphics/Material/MaterialManager.h"
#include "Runtime/Render/Graphics/Mesh/Mesh.h"
#include "Runtime/Render/Graphics/Mesh/MeshManager.h"
#include "Runtime/Render/RenderPipeline/RenderPipeline.h"
#include "Runtime/Scene/Scene.h"

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
	
	/**
	 * \brief	设置Mesh
	 * \param	shaderName	Shader名称
	 * \return	材质
	 */
	MaterialSptr& Renderer::SetMaterial(const std::string& shaderName)
	{
		if (m_MaterialSptr != nullptr)
		{
			//从渲染管线中删除Render
			RenderPipeline::RemoveRenderItem(this, m_MaterialSptr->GetShaderHashValue());
		}

		//创建材质
		m_MaterialSptr = MaterialManager::CreateMaterial(shaderName);

		//根据材质创建不同的Special Ocb Data
		m_FillSpecialOcbDataHandler = GetFillSpecialOcbDataHandler(m_MaterialSptr->GetShaderOcbType());

		//添加到渲染管线中
		RenderPipeline::AddRenderItem(this, m_MaterialSptr->GetShaderHashValue());

		return m_MaterialSptr;
	}

	void Renderer::OnEnable()
	{
		//添加到渲染管线中
		if (m_MaterialSptr != nullptr && m_CbvHeapIndex == -1)
		{
			RenderPipeline::AddRenderItem(this, m_MaterialSptr->GetShaderHashValue());
		}
	}

	void Renderer::OnDisable()
	{
		//从渲染管线中移除
		if (m_MaterialSptr != nullptr && m_CbvHeapIndex != -1)
		{
			RenderPipeline::RemoveRenderItem(this, m_MaterialSptr->GetShaderHashValue());
		}
	}

	void Renderer::OnDestroy()
	{
		
	}
}

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
		//ɾ��Mesh
		MeshManager::DestroyMesh(std::move(m_MeshSptr));
	}
	
	/**
	 * \brief	����Mesh
	 * \param	shaderName	Shader����
	 * \return	����
	 */
	MaterialSptr& Renderer::SetMaterial(const std::string& shaderName)
	{
		if (m_MaterialSptr != nullptr)
		{
			//����Ⱦ������ɾ��Render
			RenderPipeline::RemoveRenderItem(this, m_MaterialSptr->GetShaderHashValue());
		}

		//��������
		m_MaterialSptr = MaterialManager::CreateMaterial(shaderName);

		//���ݲ��ʴ�����ͬ��Special Ocb Data
		m_FillSpecialOcbDataHandler = GetFillSpecialOcbDataHandler(m_MaterialSptr->GetShaderOcbType());

		//��ӵ���Ⱦ������
		RenderPipeline::AddRenderItem(this, m_MaterialSptr->GetShaderHashValue());

		return m_MaterialSptr;
	}

	void Renderer::OnEnable()
	{
		//��ӵ���Ⱦ������
		if (m_MaterialSptr != nullptr && m_CbvHeapIndex == -1)
		{
			RenderPipeline::AddRenderItem(this, m_MaterialSptr->GetShaderHashValue());
		}
	}

	void Renderer::OnDisable()
	{
		//����Ⱦ�������Ƴ�
		if (m_MaterialSptr != nullptr && m_CbvHeapIndex != -1)
		{
			RenderPipeline::RemoveRenderItem(this, m_MaterialSptr->GetShaderHashValue());
		}
	}

	void Renderer::OnDestroy()
	{
		
	}
}

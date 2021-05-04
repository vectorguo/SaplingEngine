#include "MeshRenderer.h"

#include "Render/Graphics/ConstantBufferData.h"
#include "Render/Graphics/Material.h"
#include "Render/Graphics/Mesh.h"
#include "Render/Graphics/MeshFactory.h"

namespace SaplingEngine
{
	MeshRenderer::MeshRenderer() = default;
	MeshRenderer::~MeshRenderer() = default;

	/**
	 * \brief 反序列化
	 * \param pNode 配置节点指针
	 * \return 反序列化是否成功
	 */
	bool MeshRenderer::Deserialize(const XmlNode* pNode)
	{
		//创建Mesh
		m_pMesh = MeshFactory::CreateMesh(XmlGetAttributeValue<const char*>(pNode, "meshName"));
		
		//创建材质
		m_pMaterial = new Material();
		m_pMaterial->Deserialize(pNode->first_node("material"));

		//根据材质创建不同的Special Ocb Data
		m_FillSpecialOcbDataHandler = GetFillSpecialOcbDataHandler(m_pMaterial->GetShaderType());
		
		return true;
	}
	
	/**
	 * \brief	设置Mesh
	 * \param	shaderName	Shader名称
	 * \return	材质
	 */
	Material* MeshRenderer::SetMaterial(const std::string& shaderName)
	{
		if (m_pMaterial != nullptr)
		{
			delete m_pMaterial;
		}

		//创建材质
		m_pMaterial = new Material();
		m_pMaterial->SetShader(shaderName);

		//根据材质创建不同的Special Ocb Data
		m_FillSpecialOcbDataHandler = GetFillSpecialOcbDataHandler(m_pMaterial->GetShaderType());

		return m_pMaterial;
	}

	/**
	 * \brief	设置Mesh
	 * \param	path		Mesh资源路径
	 */
	void MeshRenderer::SetMesh(const std::string& path)
	{
		m_pMesh = MeshFactory::CreateMesh(path);
	}
}

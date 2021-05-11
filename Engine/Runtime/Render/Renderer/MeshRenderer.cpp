#include "MeshRenderer.h"

#include "Render/Graphics/ConstantBufferData.h"
#include "Render/Graphics/Material.h"
#include "Render/Graphics/MaterialManager.h"
#include "Render/Graphics/Mesh.h"
#include "Render/Graphics/MeshFactory.h"

namespace SaplingEngine
{
	MeshRenderer::MeshRenderer() = default;
	MeshRenderer::~MeshRenderer() = default;
	
	/**
	 * \brief	设置Mesh
	 * \param	shaderName	Shader名称
	 * \return	材质
	 */
	MaterialSptr& MeshRenderer::SetMaterial(const std::string& shaderName)
	{
		//创建材质
		m_MaterialSptr = MaterialManager::CreateMaterial(shaderName);

		//根据材质创建不同的Special Ocb Data
		m_FillSpecialOcbDataHandler = GetFillSpecialOcbDataHandler(m_MaterialSptr->GetShaderType());

		return m_MaterialSptr;
	}

	/**
	 * \brief	设置Mesh
	 * \param	path		Mesh资源路径
	 */
	void MeshRenderer::SetMesh(const std::string& path)
	{
		m_MeshSptr = MeshFactory::CreateMesh(path);
	}

	/**
	 * \brief 反序列化
	 * \param pNode 配置节点指针
	 * \return 反序列化是否成功
	 */
	bool MeshRenderer::Deserialize(const XmlNode* pNode)
	{
		//创建材质
		m_MaterialSptr = MaterialManager::GetSharedMaterial(XmlGetAttributeValue<const char*>(pNode, "material"));

		//根据材质创建不同的Special Ocb Data
		m_FillSpecialOcbDataHandler = GetFillSpecialOcbDataHandler(m_MaterialSptr->GetShaderType());

		//创建Mesh
		m_MeshSptr = MeshFactory::CreateMesh(XmlGetAttributeValue<const char*>(pNode, "mesh"));

		return true;
	}
}

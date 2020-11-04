#include "MeshRenderer.h"
#include "Graphics/Material.h"
#include "Graphics/Mesh.h"

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
		m_pMesh = new Mesh();
		m_pMesh->Load(XmlGetAttributeValue<const char*>(pNode, "meshName"));
		
		//创建材质
		//TODO
		
		return true;
	}
}

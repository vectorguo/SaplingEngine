#include "MeshRenderer.h"
#include "Graphics/Mesh.h"

namespace SaplingEngine
{
	MeshRenderer::MeshRenderer() = default;

	MeshRenderer::~MeshRenderer()
	{
		//释放Mesh
		delete m_pMesh;
	}

	/**
	 * \brief 反序列化
	 * \param pNode 配置节点指针
	 * \return 反序列化是否成功
	 */
	bool MeshRenderer::Deserialize(const XmlNode* pNode)
	{
		//创建Mesh
		const auto isPrimitive = XmlGetAttributeValue<bool>(pNode, "isPrimitive");
		if (isPrimitive)
		{
			//创建基本类型的Mesh
			m_pMesh = Mesh::CreatePrimitive(static_cast<EMeshPrimitiveType>(XmlGetAttributeValue<int32_t>(pNode, "primitiveType")));
		}
		else
		{
			//非基本类型的Mesh
		}

		//创建材质
		//TODO
		
		return true;
	}
}

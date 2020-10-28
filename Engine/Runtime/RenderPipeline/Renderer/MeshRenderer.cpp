#include "MeshRenderer.h"
#include "Graphics/Material.h"
#include "Graphics/Mesh.h"

namespace SaplingEngine
{
	MeshRenderer::MeshRenderer() = default;
	MeshRenderer::~MeshRenderer() = default;

	/**
	 * \brief �����л�
	 * \param pNode ���ýڵ�ָ��
	 * \return �����л��Ƿ�ɹ�
	 */
	bool MeshRenderer::Deserialize(const XmlNode* pNode)
	{
		//����Mesh
		const auto isPrimitive = XmlGetAttributeValue<bool>(pNode, "isPrimitive");
		if (isPrimitive)
		{
			//�����������͵�Mesh
			m_pMesh = Mesh::CreatePrimitive(static_cast<EMeshPrimitiveType>(XmlGetAttributeValue<int32_t>(pNode, "primitiveType")));
		}
		else
		{
			//�ǻ������͵�Mesh
		}

		//��������
		//TODO
		
		return true;
	}
}

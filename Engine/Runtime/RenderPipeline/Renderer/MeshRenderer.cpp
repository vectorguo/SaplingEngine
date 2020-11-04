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
		m_pMesh = new Mesh();
		m_pMesh->Load(XmlGetAttributeValue<const char*>(pNode, "meshName"));
		
		//��������
		//TODO
		
		return true;
	}
}

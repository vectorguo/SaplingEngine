#include "MeshRenderer.h"

#include "Render/Graphics/ConstantBufferData.h"
#include "Render/Graphics/Material.h"
#include "Render/Graphics/Mesh.h"

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
		m_pMaterial = new Material();
		m_pMaterial->Deserialize(pNode->first_node("material"));

		//TODO ���ݲ��ʴ�����ͬ��Special Ocb Data
		m_pSpecialOcbData = new SpecialOcbData();
		
		return true;
	}
}

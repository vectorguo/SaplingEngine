#include "MeshRenderer.h"

#include "Runtime/Render/Graphics/ConstantBufferData.h"
#include "Runtime/Render/Graphics/Material/Material.h"
#include "Runtime/Render/Graphics/Material/MaterialManager.h"
#include "Runtime/Render/Graphics/Mesh/Mesh.h"
#include "Runtime/Render/Graphics/Mesh/MeshManager.h"

namespace SaplingEngine
{
	MeshRenderer::MeshRenderer() = default;
	MeshRenderer::~MeshRenderer() = default;

	/**
	 * \brief	����Mesh
	 * \param	path		Mesh��Դ·��
	 */
	void MeshRenderer::SetMesh(const std::string& path)
	{
		m_MeshSptr = MeshManager::CreateMesh(path);
	}

	/**
	 * \brief �����л�
	 * \param pNode ���ýڵ�ָ��
	 * \return �����л��Ƿ�ɹ�
	 */
	bool MeshRenderer::Deserialize(const XmlNode* pNode)
	{
		//��������
		m_MaterialSptr = MaterialManager::GetSharedMaterial(XmlGetAttributeValue<const char*>(pNode, "material"));

		//���ݲ��ʴ�����ͬ��Special Ocb Data
		m_FillSpecialOcbDataHandler = GetFillSpecialOcbDataHandler(m_MaterialSptr->GetShaderOcbType());

		//����Mesh
		m_MeshSptr = MeshManager::CreateMesh(XmlGetAttributeValue<const char*>(pNode, "mesh"));

		return true;
	}
}

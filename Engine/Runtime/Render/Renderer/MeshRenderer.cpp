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
	 * \brief	����Mesh
	 * \param	shaderName	Shader����
	 * \return	����
	 */
	MaterialSptr& MeshRenderer::SetMaterial(const std::string& shaderName)
	{
		//��������
		m_MaterialSptr = MaterialManager::CreateMaterial(shaderName);

		//���ݲ��ʴ�����ͬ��Special Ocb Data
		m_FillSpecialOcbDataHandler = GetFillSpecialOcbDataHandler(m_MaterialSptr->GetShaderType());

		return m_MaterialSptr;
	}

	/**
	 * \brief	����Mesh
	 * \param	path		Mesh��Դ·��
	 */
	void MeshRenderer::SetMesh(const std::string& path)
	{
		m_MeshSptr = MeshFactory::CreateMesh(path);
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
		m_FillSpecialOcbDataHandler = GetFillSpecialOcbDataHandler(m_MaterialSptr->GetShaderType());

		//����Mesh
		m_MeshSptr = MeshFactory::CreateMesh(XmlGetAttributeValue<const char*>(pNode, "mesh"));

		return true;
	}
}

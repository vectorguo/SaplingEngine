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
	 * \brief �����л�
	 * \param pNode ���ýڵ�ָ��
	 * \return �����л��Ƿ�ɹ�
	 */
	bool MeshRenderer::Deserialize(const XmlNode* pNode)
	{
		//����Mesh
		m_pMesh = MeshFactory::CreateMesh(XmlGetAttributeValue<const char*>(pNode, "meshName"));
		
		//��������
		m_pMaterial = new Material();
		m_pMaterial->Deserialize(pNode->first_node("material"));

		//���ݲ��ʴ�����ͬ��Special Ocb Data
		m_FillSpecialOcbDataHandler = GetFillSpecialOcbDataHandler(m_pMaterial->GetShaderType());
		
		return true;
	}
	
	/**
	 * \brief	����Mesh
	 * \param	shaderName	Shader����
	 * \return	����
	 */
	Material* MeshRenderer::SetMaterial(const std::string& shaderName)
	{
		if (m_pMaterial != nullptr)
		{
			delete m_pMaterial;
		}

		//��������
		m_pMaterial = new Material();
		m_pMaterial->SetShader(shaderName);

		//���ݲ��ʴ�����ͬ��Special Ocb Data
		m_FillSpecialOcbDataHandler = GetFillSpecialOcbDataHandler(m_pMaterial->GetShaderType());

		return m_pMaterial;
	}

	/**
	 * \brief	����Mesh
	 * \param	path		Mesh��Դ·��
	 */
	void MeshRenderer::SetMesh(const std::string& path)
	{
		m_pMesh = MeshFactory::CreateMesh(path);
	}
}

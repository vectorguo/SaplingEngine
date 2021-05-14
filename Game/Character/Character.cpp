#include "Character.h"

namespace Warcraft
{
	/**
	 * \brief	�����л�
	 * \param	pNode		���ýڵ�ָ��
	 * \return	�����л��Ƿ�ɹ�
	 */
	bool Character::Deserialize(const XmlNode* pNode)
	{
		return false;
	}
	
	/**
	 * \brief	Start
	 */
	void Character::Start()
	{
		auto meshRenderer = m_GameObjectPtr->AddComponent<MeshRenderer>();
		auto& material = meshRenderer->SetMaterial("Standard");
		material->SetColor("baseColor", Color::Red);
		material->SetVector3("fresnel", Vector3(0.01f, 0.01f, 0.01f));
		material->SetFloat("roughtness", 0.5f);

		meshRenderer->SetMesh("Resources/Characters/player.obj");

		//���ó�ʼλ��
		GetTransform()->SetPosition(0, 0, -120.0f);
	}

	/**
	 * \brief	����
	 */
	void Character::Update()
	{
	}
}
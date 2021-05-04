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
		auto meshRenderer = m_GameObjectSptr->AddComponent<MeshRenderer>();
		auto* pMaterial = meshRenderer->SetMaterial("Opaque");
		pMaterial->SetColor("baseColor", Color::Red);

		meshRenderer->SetMesh("player");
	}

	/**
	 * \brief	����
	 */
	void Character::Update()
	{
	}
}
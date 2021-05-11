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
		auto& material = meshRenderer->SetMaterial("Opaque");
		material->SetColor("baseColor", Color::Red);

		meshRenderer->SetMesh("Resources/Characters/player.obj");
	}

	/**
	 * \brief	����
	 */
	void Character::Update()
	{
	}
}
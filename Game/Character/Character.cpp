#include "Character.h"

namespace Warcraft
{
	/**
	 * \brief	反序列化
	 * \param	pNode		配置节点指针
	 * \return	反序列化是否成功
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
	 * \brief	更新
	 */
	void Character::Update()
	{
	}
}
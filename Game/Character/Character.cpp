#include "Game/Character/Character.h"

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
		auto& material = meshRenderer->SetMaterial("Standard");
		material->SetColor("baseColor", Color::Red);
		material->SetVector3("fresnel", Vector3(0.01f, 0.01f, 0.01f));
		material->SetFloat("roughtness", 0.5f);

		meshRenderer->SetMesh("Resources/Characters/player.obj");

		//设置初始位置
		GetTransform()->SetPosition(0, 0, -120);

		//设置场景包围盒中心点
		auto* pActiveScene = SceneManager::GetActiveScene();
		pActiveScene->SetCenterOfSceneBounds(GetTransform()->GetPosition());
	}

	/**
	 * \brief	更新
	 */
	void Character::Update()
	{
	}
}
#include "Game/Input/InputSystem.h"
#include "Game/WarcraftGameMain.h"

namespace Warcraft
{
	WarcraftGameMain* WarcraftGameMain::pInstance = nullptr;

	/**
	 * \brief	反序列化
	 * \param	pNode		配置节点指针
	 * \return	反序列化是否成功
	 */
	bool WarcraftGameMain::Deserialize(const XmlNode* pNode)
	{
		return true;
	}
	
	void WarcraftGameMain::Start()
	{
		//创建玩家角色
		auto pCharacterObj = CreateGameObject("Player");
		pCharacterObj->GetTransform()->SetPosition(80.0f, 0, -70.0f);
		m_PlayerSptr = pCharacterObj->AddComponent<Character>();

		//初始化InputSystem
		InputSystem::Initialize(m_PlayerSptr.Get());
	}
	
	void WarcraftGameMain::Update()
	{
		//InputSystem更新
		InputSystem::Update();
	}
}
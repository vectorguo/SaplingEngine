#include "Game/Input/InputSystem.h"
#include "Game/WarcraftGameMain.h"

namespace Warcraft
{
	WarcraftGameMain* WarcraftGameMain::pInstance = nullptr;

	/**
	 * \brief	�����л�
	 * \param	pNode		���ýڵ�ָ��
	 * \return	�����л��Ƿ�ɹ�
	 */
	bool WarcraftGameMain::Deserialize(const XmlNode* pNode)
	{
		return true;
	}
	
	void WarcraftGameMain::Start()
	{
		//������ҽ�ɫ
		auto pCharacterObj = CreateGameObject("Player");
		pCharacterObj->GetTransform()->SetPosition(80.0f, 0, -70.0f);
		m_PlayerSptr = pCharacterObj->AddComponent<Character>();

		//��ʼ��InputSystem
		InputSystem::Initialize(m_PlayerSptr.Get());
	}
	
	void WarcraftGameMain::Update()
	{
		//InputSystem����
		InputSystem::Update();
	}
}
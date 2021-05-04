#include "Input/InputSystem.h"
#include "WarcraftGameMain.h"

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
		auto* pActiveScene = SaplingEngine::SceneManager::GetActiveScene();

		//������ҽ�ɫ
		auto pCharacterObj = pActiveScene->CreateGameObject("Player");
		m_PlayerSptr = pCharacterObj->AddComponent<Character>();

		//��ʼ��InputSystem
		InputSystem::Initialize(m_PlayerSptr.get());
	}
	
	void WarcraftGameMain::Update()
	{
		//InputSystem����
		InputSystem::Update();
	}
}
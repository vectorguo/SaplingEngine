#include "ChessWarInput.h"
#include "Camera/Camera.h"

namespace ChessWar
{
	/**
	 * \brief �����л�
	 * \param pNode ���ýڵ�ָ��
	 * \return �����л��Ƿ�ɹ�
	 */
	bool ChessWarInput::Deserialize(const XmlNode* pNode)
	{
		return true;
	}

	/**
	 * \brief Start
	 */
	void ChessWarInput::Start()
	{
		m_MainCamera = m_pGameObject->GetComponent<SaplingEngine::Camera>();
	}

	/**
	 * \brief ����
	 */
	void ChessWarInput::Update()
	{
		//�������
		UpdateCamera();
	}
	
	/**
	 * \brief �������
	 */
	void ChessWarInput::UpdateCamera()
	{
		
	}
}

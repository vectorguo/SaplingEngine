#include "ChessWarInput.h"
#include "Camera/Camera.h"

namespace ChessWar
{
	/**
	 * \brief 反序列化
	 * \param pNode 配置节点指针
	 * \return 反序列化是否成功
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
	 * \brief 更新
	 */
	void ChessWarInput::Update()
	{
		//更新相机
		UpdateCamera();
	}
	
	/**
	 * \brief 更新相机
	 */
	void ChessWarInput::UpdateCamera()
	{
		
	}
}

#include "ChessWarInput.h"
#include "Camera/Camera.h"
#include "Input/Input.h"

using namespace SaplingEngine;
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
		m_MainCamera = m_pGameObject->GetComponent<Camera>();
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
		const auto* pInput = Input::Instance();
		const auto mouseState = pInput->GetMouseButtonState();
		if (mouseState == EMouseButtonState::LeftMouseButtonDown)
		{
			m_MousePosition = pInput->GetMousePosition();
		}
		else if (mouseState == EMouseButtonState::MouseButtonMove)
		{
			const auto& mousePosition = pInput->GetMousePosition();
			const auto delta = mousePosition - m_MousePosition;

			auto* pCameraTransform = m_MainCamera->GetTransform();
			auto cameraRotation = pCameraTransform->GetLocalRotation();
			cameraRotation *= Quaternion::RotateAxis(Vector3::Up, delta.x);
			pCameraTransform->SetLocalRotation(cameraRotation);
			
			// auto* pCameraTransform = m_MainCamera->GetTransform();
			// auto cameraPosition = pCameraTransform->GetLocalPosition();
			// cameraPosition.z += delta.x;
			// pCameraTransform->SetLocalPosition(cameraPosition);
			// Log::LogInfo(std::to_string(cameraPosition.z));
			
			m_MousePosition = pInput->GetMousePosition();
		}
	}
}

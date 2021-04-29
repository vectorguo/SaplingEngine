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
		m_MainCamera = m_GameObjectSptr->GetComponent<Camera>();
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
		const auto mouseState = Input::GetMouseButtonState();
		if (mouseState == EMouseButtonState::MouseWheel)
		{
			auto* pCameraTransform = m_MainCamera->GetTransform();
			auto cameraPosition = pCameraTransform->GetLocalPosition();
			cameraPosition += pCameraTransform->GetForward() * Input::GetMouseWheelValue() * 2.0f;
			pCameraTransform->SetLocalPosition(cameraPosition);
		}
		else
		{
			if (mouseState == EMouseButtonState::LeftMouseButtonDown)
			{
				m_CameraOperation = 1;
				m_MousePosition = Input::GetMousePosition();
			}
			else if (mouseState == EMouseButtonState::RightMouseButtonDown)
			{
				m_CameraOperation = 2;
				m_MousePosition = Input::GetMousePosition();
			}
			else if (mouseState == EMouseButtonState::LeftMouseButtonUp || mouseState == EMouseButtonState::RightMouseButtonUp)
			{
				m_CameraOperation = 0;
			}

			if ((m_CameraOperation == 1 || m_CameraOperation == 2) && mouseState == EMouseButtonState::MouseButtonMove)
			{
				const auto& mousePosition = Input::GetMousePosition();
				const auto delta = m_MousePosition - mousePosition;
				auto* pCameraTransform = m_MainCamera->GetTransform();

				if (m_CameraOperation == 1)
				{
					//平移相机
					auto cameraPosition = pCameraTransform->GetLocalPosition();
					cameraPosition += pCameraTransform->GetRight() * delta.x * 0.05f;
					cameraPosition += pCameraTransform->GetUp() * -delta.y * 0.05f;
					pCameraTransform->SetLocalPosition(cameraPosition);
				}
				else
				{
					//旋转相机
					auto cameraRotation = pCameraTransform->GetLocalRotation();
					cameraRotation *= Quaternion::RotateAxis(pCameraTransform->GetRight(), delta.y * 0.1f);
					cameraRotation *= Quaternion::RotateAxis(Vector3::Up, delta.x * 0.1f);
					pCameraTransform->SetLocalRotation(cameraRotation);
				}

				m_MousePosition = Input::GetMousePosition();
			}
		}
	}
}

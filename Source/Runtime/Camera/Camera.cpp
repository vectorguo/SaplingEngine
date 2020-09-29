#include "Camera.h"
#include "CameraManager.h"

namespace SaplingEngine
{
	void Camera::Awake()
	{
		CameraManager::Instance()->AddCamera(std::static_pointer_cast<Camera>(shared_from_this()));
	}

	void Camera::OnDestroy()
	{
		CameraManager::Instance()->RemoveCamera(std::static_pointer_cast<Camera>(shared_from_this()));
	}

	/**
	 * \brief ����������ȼ�
	 * \param priority ���ȼ�
	 */
	void Camera::SetPriority(const int priority)
	{
		m_Priority = priority;

		//�������������
		CameraManager::Instance()->SortCamera();
	}
}

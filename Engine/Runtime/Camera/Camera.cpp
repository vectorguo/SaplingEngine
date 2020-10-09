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
	 * \brief 设置相机优先级
	 * \param priority 优先级
	 */
	void Camera::SetPriority(const int priority)
	{
		m_Priority = priority;

		//对相机进行排序
		CameraManager::Instance()->SortCamera();
	}
}

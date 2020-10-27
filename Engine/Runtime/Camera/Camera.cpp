#include "Camera.h"
#include "CameraManager.h"

namespace SaplingEngine
{
	/**
	 * \brief 反序列化
	 * \param pNode 配置节点指针
	 * \return 反序列化是否成功
	 */
	bool Camera::Deserialize(const XmlNode* pNode)
	{
		m_Projection = static_cast<EProjection>(XmlGetAttributeValue<int32_t>(pNode, "projection"));
		if (m_Projection == EProjection::Perspective)
		{
			//透视投影
			m_Fov = XmlGetAttributeValue<float>(pNode, "fov");
			m_NearClippingPlanes = XmlGetAttributeValue<float>(pNode, "near");
			m_FarClippingPlanes = XmlGetAttributeValue<float>(pNode, "far");
		}
		else
		{
		}

		m_Priority = XmlGetAttributeValue<int32_t>(pNode, "priority");
		m_CullingMask = XmlGetAttributeValue<int32_t>(pNode, "cullingMask");

		return true;
	}

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

#include "Runtime/Application/Setting.h"
#include "Runtime/Camera/Camera.h"
#include "Runtime/Camera/CameraManager.h"
#include "Runtime/GameObject/GameObject.h"

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
		//初始化窗口参数
		m_WindowWidth = static_cast<float>(Setting::ScreenWidth());
		m_WindowHeight = static_cast<float>(Setting::ScreenHeight());

		//添加相机到CameraManager
		CameraManager::AddCamera(this);
	}

	void Camera::OnDestroy()
	{
		//从CameraManager中删除相机
		CameraManager::RemoveCamera(this);
	}

	/**
	 * \brief 刷新矩阵
	 */
	void Camera::RefreshMatrix()
	{
		//计算视图矩阵
		m_WorldToViewMatrix = GetTransform()->GetWorldToLocalMatrix();

		if (m_IsDirty)
		{
			//计算投影矩阵
			m_ViewToProjMatrix = m_Projection == EProjection::Orthographic
				? Matrix4x4::Orthographic(m_WindowWidth, m_WindowHeight, m_NearClippingPlanes, m_FarClippingPlanes)
				: Matrix4x4::Perspective(m_Fov, m_WindowWidth / m_WindowHeight, m_NearClippingPlanes, m_FarClippingPlanes);
			
			m_IsDirty = false;
		}
	}

	/**
	 * \brief 设置相机优先级
	 * \param priority 优先级
	 */
	void Camera::SetPriority(const int priority)
	{
		m_Priority = priority;

		//对相机进行排序
		CameraManager::SortCamera();
	}
}

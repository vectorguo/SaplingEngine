#include "Runtime/Application/Setting.h"
#include "Runtime/Camera/Camera.h"
#include "Runtime/Camera/CameraManager.h"
#include "Runtime/GameObject/GameObject.h"

namespace SaplingEngine
{
	/**
	 * \brief �����л�
	 * \param pNode ���ýڵ�ָ��
	 * \return �����л��Ƿ�ɹ�
	 */
	bool Camera::Deserialize(const XmlNode* pNode)
	{
		m_Projection = static_cast<EProjection>(XmlGetAttributeValue<int32_t>(pNode, "projection"));
		if (m_Projection == EProjection::Perspective)
		{
			//͸��ͶӰ
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
		//��ʼ�����ڲ���
		m_WindowWidth = static_cast<float>(Setting::ScreenWidth());
		m_WindowHeight = static_cast<float>(Setting::ScreenHeight());

		//��������CameraManager
		CameraManager::AddCamera(this);
	}

	void Camera::OnDestroy()
	{
		//��CameraManager��ɾ�����
		CameraManager::RemoveCamera(this);
	}

	/**
	 * \brief ˢ�¾���
	 */
	void Camera::RefreshMatrix()
	{
		//������ͼ����
		m_WorldToViewMatrix = GetTransform()->GetWorldToLocalMatrix();

		if (m_IsDirty)
		{
			//����ͶӰ����
			m_ViewToProjMatrix = m_Projection == EProjection::Orthographic
				? Matrix4x4::Orthographic(m_WindowWidth, m_WindowHeight, m_NearClippingPlanes, m_FarClippingPlanes)
				: Matrix4x4::Perspective(m_Fov, m_WindowWidth / m_WindowHeight, m_NearClippingPlanes, m_FarClippingPlanes);
			
			m_IsDirty = false;
		}
	}

	/**
	 * \brief ����������ȼ�
	 * \param priority ���ȼ�
	 */
	void Camera::SetPriority(const int priority)
	{
		m_Priority = priority;

		//�������������
		CameraManager::SortCamera();
	}
}

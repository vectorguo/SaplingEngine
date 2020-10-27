#include "Camera.h"
#include "CameraManager.h"

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

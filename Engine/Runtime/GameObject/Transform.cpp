#include "GameObject.h"
#include "Transform.h"

namespace SaplingEngine
{
	Transform::Transform() :
		Component(ComponentType_Transform),
		m_Position(0, 0, 0),
		m_LocalPosition(0, 0, 0),
		m_Rotation(Quaternion::Identity),
		m_LocalRotation(Quaternion::Identity),
		m_LocalScale(1, 1, 1)
	{

	}

	/**
	 * \brief �����л�
	 * \param pNode ���ýڵ�ָ��
	 * \return �����л��Ƿ�ɹ�
	 */
	bool Transform::Deserialize(const XmlNode* pNode)
	{
		m_LocalPosition.Set(XmlGetAttributeValue<float>(pNode, "lpx"), XmlGetAttributeValue<float>(pNode, "lpy"), XmlGetAttributeValue<float>(pNode, "lpz"));
		m_LocalRotation.Set(XmlGetAttributeValue<float>(pNode, "lrx"), XmlGetAttributeValue<float>(pNode, "lry"), XmlGetAttributeValue<float>(pNode, "lrz"), XmlGetAttributeValue<float>(pNode, "lrw"));
		m_LocalScale.Set(XmlGetAttributeValue<float>(pNode, "lsx"), XmlGetAttributeValue<float>(pNode, "lsy"), XmlGetAttributeValue<float>(pNode, "lsz"));

		return true;
	}

	/**
	* \brief	��ȡλ��
	* \return	λ�ã������޸ģ�
	*/
	const Vector3& Transform::GetPosition()
	{
		if (IsDirty(0x04))
		{
			//�ֲ�����ϵ�µ�λ���и��£���Ҫ���ֲ�����ϵ�µ�λ��ת������������ϵ�µ�λ��
			auto& localToWorldMatrix = m_GameObjectPtr->GetParent()->GetTransform()->GetLocalToWorldMatrix();
			m_Position = localToWorldMatrix.MultiplyPoint(m_LocalPosition);

			//�������
			SetDirty(0x04, false);
		}
		return m_Position;
	}

	/**
	* \brief	��ȡ��ת
	* \return	��ת�������޸ģ�
	*/
	const Quaternion& Transform::GetRotation()
	{
		if (IsDirty(0x08))
		{
			//�ֲ�����ϵ�µ���ת�и��£���Ҫ���ֲ�����ϵ�µ���תת������������ϵ�µ���ת
			auto& parentRotation = m_GameObjectPtr->GetParent()->GetTransform()->GetRotation();
			m_Rotation = parentRotation * m_LocalRotation;

			//�������
			SetDirty(0x08, false);
		}
		return m_Rotation;
	}

	/**
	* \brief	��ȡ�ֲ������µ�λ��
	* \return	�ֲ������µ�λ�ã������޸ģ�
	*/
	const Vector3& Transform::GetLocalPosition()
	{
		if (IsDirty(0x01))
		{
			//��������ϵ�µ�λ���и��£���Ҫ����������ϵ�µ�λ��ת���ɾֲ�����ϵ�µ�λ��
			auto& worldToLocalMatrix = m_GameObjectPtr->GetParent()->GetTransform()->GetWorldToLocalMatrix();
			m_LocalPosition = worldToLocalMatrix.MultiplyPoint(m_Position);

			//�������
			SetDirty(0x01, false);
		}
		return m_LocalPosition;
	}

	/**
	* \brief	��ȡ�ֲ������µ���ת
	* \return	�ֲ������µ���ת�������޸ģ�
	*/
	const Quaternion& Transform::GetLocalRotation()
	{
		if (IsDirty(0x02))
		{
			//��������ϵ�µ���ת�и��£���Ҫ����������ϵ�µ���תת���ɾֲ�����ϵ�µ���ת
			auto& parentRotation = m_GameObjectPtr->GetParent()->GetTransform()->GetRotation();
			m_LocalRotation = parentRotation.Inverse() * m_Rotation;

			//�������
			SetDirty(0x02, false);
		}
		return m_LocalRotation;
	}

	/**
	* \brief	��ȡ����
	* \return	���ţ������޸ģ�
	*/
	const Vector3& Transform::GetLocalScale()
	{
		return m_LocalScale;
	}

	/**
	* \brief	����λ��
	* \param	position		λ��
	*/
	void Transform::SetPosition(const Vector3& position)
	{
		m_Position = position;
		if (m_GameObjectPtr->HasParent())
		{
			SetDirty(0x01, true);
			SetDirty(0x04, false);
			SetDirty(0x20, true);
		}
		else
		{
			SetDirty(0x20, true);
			m_LocalPosition = position;
		}
	}

	/**
	* \brief	������ת
	* \param	rotation		��ת
	*/
	void Transform::SetRotation(const Quaternion& rotation)
	{
		m_Rotation = rotation;
		if (m_GameObjectPtr->HasParent())
		{
			SetDirty(0x02, true);
			SetDirty(0x08, false);
			SetDirty(0x20, true);
		}
		else
		{
			SetDirty(0x20, true);
			m_LocalRotation = rotation;
		}
	}

	/**
	* \brief	���þֲ������µ�λ��
	* \param	localPosition	�ֲ������µ�λ��
	*/
	void Transform::SetLocalPosition(const Vector3& localPosition)
	{
		m_LocalPosition = localPosition;
		if (m_GameObjectPtr->HasParent())
		{
			SetDirty(0x01, false);
			SetDirty(0x04, true);
			SetDirty(0x20, true);
		}
		else
		{
			SetDirty(0x20, true);
			m_Position = localPosition;
		}
	}

	/**
	* \brief	���þֲ������µ���ת
	* \param	localRotation	�ֲ������µ���ת
	*/
	void Transform::SetLocalRotation(const Quaternion& localRotation)
	{
		m_LocalRotation = localRotation;
		if (m_GameObjectPtr->HasParent())
		{
			SetDirty(0x02, false);
			SetDirty(0x08, true);
			SetDirty(0x20, true);
		}
		else
		{
			SetDirty(0x20, true);
			m_Rotation = localRotation;
		}
	}

	/**
	* \brief	��������
	* \param	localScale		����
	*/
	void Transform::SetLocalScale(const Vector3& localScale)
	{
		m_LocalScale = localScale;
		if (m_GameObjectPtr->HasParent())
		{
			SetDirty(0x10, true);
			SetDirty(0x20, true);
		}
		else
		{
			SetDirty(0x20, true);
		}
	}

	/**
	 * \brief ˢ�±任����
	 */
	void Transform::RefreshMatrix()
	{
		if (IsDirty(0x20))
		{
			if (IsDirty(0x01))
			{
				//��������ϵ�µ�λ���и��£���Ҫ����������ϵ�µ�λ��ת���ɾֲ�����ϵ�µ�λ��
				auto& worldToLocalMatrix = m_GameObjectPtr->GetParent()->GetTransform()->GetWorldToLocalMatrix();
				m_LocalPosition = worldToLocalMatrix.MultiplyPoint(m_Position);

				//�������
				SetDirty(0x01, false);
			}

			if (IsDirty(0x02))
			{
				//��������ϵ�µ���ת�и��£���Ҫ����������ϵ�µ���תת���ɾֲ�����ϵ�µ���ת
				auto& parentRotation = m_GameObjectPtr->GetParent()->GetTransform()->GetRotation();
				m_LocalRotation = parentRotation.Inverse() * m_Rotation;

				//�������
				SetDirty(0x02, false);
			}

			m_LocalToWorldMatrix = Matrix4x4::Scale(m_LocalScale) * Matrix4x4::Rotate(m_LocalRotation) * Matrix4x4::Translate(m_LocalPosition);
			if (m_GameObjectPtr->HasParent())
			{
				m_LocalToWorldMatrix *= m_GameObjectPtr->GetParentSptr()->GetTransform()->GetLocalToWorldMatrix();
			}
			m_WorldToLocalMatrix = m_LocalToWorldMatrix.Inverse();

			SetDirty(0x20, false);
		}
	}
}

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
	 * \brief	��ȡλ��
	 * \return	λ�ã������޸ģ�
	 */
	const Vector3& Transform::GetPosition()
	{
		if (IsDirty(0x04))
		{
			//�ֲ�����ϵ�µ�λ���и��£���Ҫ���ֲ�����ϵ�µ�λ��ת������������ϵ�µ�λ��
			m_Position = m_Parent->GetLocalToWorldMatrix().MultiplyPoint(m_LocalPosition);

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
			m_Rotation = m_Parent->GetRotation() * m_LocalRotation;

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
			m_LocalPosition = m_Parent->GetWorldToLocalMatrix().MultiplyPoint(m_Position);

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
			m_LocalRotation = m_Parent->GetRotation().Inverse() * m_Rotation;

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
	 * \brief	��ȡforward����
	 * \return	forward����
	 */
	Vector3 Transform::GetForward()
	{
		auto forward = GetRotation() * Vector3::Forward;
		forward.Normalize();
		return forward;
	}

	/**
	 * \brief	��ȡright����
	 * \return	right����
	 */
	Vector3 Transform::GetRight()
	{
		auto right = GetRotation() * Vector3::Right;
		right.Normalize();
		return right;
	}

	/**
	 * \brief	��ȡup����
	 * \return	up����
	 */
	Vector3 Transform::GetUp()
	{
		auto up = GetRotation() * Vector3::Up;
		up.Normalize();
		return up;
	}

	/**
	 * \brief	����λ��
	 * \param	position		λ��
	 */
	void Transform::SetPosition(const Vector3& position)
	{
		m_Position = position;
		if (HasParent())
		{
			SetDirty(0x01, true);
			SetDirty(0x04, false);
			SetDirty(0x20, true);
		}
		else
		{
			SetDirty(0x20, true);
			m_LocalPosition = m_Position;
		}
	}

	/**
	 * \brief	����λ��
	 * \param	x				x����
	 * \param	y				y����
	 * \param	z				z����
	 */
	void Transform::SetPosition(float x, float y, float z)
	{
		m_Position.Set(x, y, z);
		if (HasParent())
		{
			SetDirty(0x01, true);
			SetDirty(0x04, false);
			SetDirty(0x20, true);
		}
		else
		{
			SetDirty(0x20, true);
			m_LocalPosition = m_Position;
		}
	}

	/**
	 * \brief	������ת
	 * \param	rotation		��ת
	 */
	void Transform::SetRotation(const Quaternion& rotation)
	{
		m_Rotation = rotation;
		if (HasParent())
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
		if (HasParent())
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
	 * \brief	���þֲ������µ�λ��
	 * \param	x				x����
	 * \param	y				y����
	 * \param	z				z����
	 */
	void Transform::SetLocalPosition(float x, float y, float z)
	{
		m_LocalPosition.Set(x, y, z);
		if (HasParent())
		{
			SetDirty(0x01, false);
			SetDirty(0x04, true);
			SetDirty(0x20, true);
		}
		else
		{
			SetDirty(0x20, true);
			m_Position = m_LocalPosition;
		}
	}

	/**
	 * \brief	���þֲ������µ���ת
	 * \param	localRotation	�ֲ������µ���ת
	 */
	void Transform::SetLocalRotation(const Quaternion& localRotation)
	{
		m_LocalRotation = localRotation;
		if (HasParent())
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

	void Transform::SetLocalRotation(float x, float y, float z, float w)
	{
		m_LocalRotation.Set(x, y, z, w);
		if (HasParent())
		{
			SetDirty(0x02, false);
			SetDirty(0x08, true);
			SetDirty(0x20, true);
		}
		else
		{
			SetDirty(0x20, true);
			m_Rotation = m_LocalRotation;
		}
	}

	/**
	 * \brief	��������
	 * \param	localScale		����
	 */
	void Transform::SetLocalScale(const Vector3& localScale)
	{
		m_LocalScale = localScale;
		if (HasParent())
		{
			SetDirty(0x10, true);
			SetDirty(0x20, true);
		}
		else
		{
			SetDirty(0x20, true);
		}
	}

	void Transform::SetLocalScale(float x, float y, float z)
	{
		m_LocalScale.Set(x, y, z);
		if (HasParent())
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
	 * \brief	����parent
	 * \param	pParent			parentָ��
	 */
	void Transform::SetParent(Transform* pParent)
	{
		if (pParent == m_Parent)
		{
			return;
		}

		if (m_Parent)
		{
			m_Parent->RemoveChild(this);
		}
		m_Parent = pParent;

		if (m_Parent)
		{
			m_Parent->AddChild(this);
		}
	}

	/**
	 * \brief �����л�
	 * \param pNode ���ýڵ�ָ��
	 * \return �����л��Ƿ�ɹ�
	 */
	bool Transform::Deserialize(const XmlNode* pNode)
	{
		SetLocalPosition(XmlGetAttributeValue<float>(pNode, "lpx"), XmlGetAttributeValue<float>(pNode, "lpy"), XmlGetAttributeValue<float>(pNode, "lpz"));
		SetLocalRotation(XmlGetAttributeValue<float>(pNode, "lrx"), XmlGetAttributeValue<float>(pNode, "lry"), XmlGetAttributeValue<float>(pNode, "lrz"), XmlGetAttributeValue<float>(pNode, "lrw"));
		SetLocalScale(XmlGetAttributeValue<float>(pNode, "lsx"), XmlGetAttributeValue<float>(pNode, "lsy"), XmlGetAttributeValue<float>(pNode, "lsz"));

		return true;
	}

	/**
	 * \brief	Transform������ʱ�Ļص�
	 */
	void Transform::OnDestroy()
	{
		if (m_Parent)
		{
			m_Parent->RemoveChild(this);
			m_Parent = nullptr;
		}

		if (!m_Children.empty())
		{
			for (auto iter = m_Children.begin(); iter != m_Children.end(); ++iter)
			{
				auto* pChild = *iter;
				pChild->m_Parent = nullptr;
			}
			m_Children.clear();
		}
	}

	/**
	 * \brief	ˢ�±任����
	 */
	void Transform::RefreshMatrix()
	{
		if (IsDirty(0x20))
		{
			if (IsDirty(0x01))
			{
				//��������ϵ�µ�λ���и��£���Ҫ����������ϵ�µ�λ��ת���ɾֲ�����ϵ�µ�λ��
				m_LocalPosition = m_Parent->GetWorldToLocalMatrix().MultiplyPoint(m_Position);

				//�������
				SetDirty(0x01, false);
			}

			if (IsDirty(0x02))
			{
				//��������ϵ�µ���ת�и��£���Ҫ����������ϵ�µ���תת���ɾֲ�����ϵ�µ���ת
				m_LocalRotation = m_Parent->GetRotation().Inverse() * m_Rotation;

				//�������
				SetDirty(0x02, false);
			}

			m_LocalToWorldMatrix = Matrix4x4::Scale(m_LocalScale) * Matrix4x4::Rotate(m_LocalRotation) * Matrix4x4::Translate(m_LocalPosition);
			if (HasParent())
			{
				m_LocalToWorldMatrix *= m_Parent->GetLocalToWorldMatrix();
			}
			m_WorldToLocalMatrix = m_LocalToWorldMatrix.Inverse();

			SetDirty(0x20, false);
		}
	}
}

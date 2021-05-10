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
	 * \brief	获取位置
	 * \return	位置（不可修改）
	 */
	const Vector3& Transform::GetPosition()
	{
		if (IsDirty(0x04))
		{
			//局部坐标系下的位置有更新，需要将局部坐标系下的位置转换成世界坐标系下的位置
			m_Position = m_Parent->GetLocalToWorldMatrix().MultiplyPoint(m_LocalPosition);

			//清除脏标记
			SetDirty(0x04, false);
		}
		return m_Position;
	}

	/**
	 * \brief	获取旋转
	 * \return	旋转（不可修改）
	 */
	const Quaternion& Transform::GetRotation()
	{
		if (IsDirty(0x08))
		{
			//局部坐标系下的旋转有更新，需要将局部坐标系下的旋转转换成世界坐标系下的旋转
			m_Rotation = m_Parent->GetRotation() * m_LocalRotation;

			//清除脏标记
			SetDirty(0x08, false);
		}
		return m_Rotation;
	}

	/**
	 * \brief	获取局部坐标下的位置
	 * \return	局部坐标下的位置（不可修改）
	 */
	const Vector3& Transform::GetLocalPosition()
	{
		if (IsDirty(0x01))
		{
			//世界坐标系下的位置有更新，需要将世界坐标系下的位置转换成局部坐标系下的位置
			m_LocalPosition = m_Parent->GetWorldToLocalMatrix().MultiplyPoint(m_Position);

			//清除脏标记
			SetDirty(0x01, false);
		}
		return m_LocalPosition;
	}

	/**
	 * \brief	获取局部坐标下的旋转
	 * \return	局部坐标下的旋转（不可修改）
	 */
	const Quaternion& Transform::GetLocalRotation()
	{
		if (IsDirty(0x02))
		{
			//世界坐标系下的旋转有更新，需要将世界坐标系下的旋转转换成局部坐标系下的旋转
			m_LocalRotation = m_Parent->GetRotation().Inverse() * m_Rotation;

			//清除脏标记
			SetDirty(0x02, false);
		}
		return m_LocalRotation;
	}

	/**
	 * \brief	获取缩放
	 * \return	缩放（不可修改）
	 */
	const Vector3& Transform::GetLocalScale()
	{
		return m_LocalScale;
	}

	/**
	 * \brief	获取forward向量
	 * \return	forward向量
	 */
	Vector3 Transform::GetForward()
	{
		auto forward = GetRotation() * Vector3::Forward;
		forward.Normalize();
		return forward;
	}

	/**
	 * \brief	获取right向量
	 * \return	right向量
	 */
	Vector3 Transform::GetRight()
	{
		auto right = GetRotation() * Vector3::Right;
		right.Normalize();
		return right;
	}

	/**
	 * \brief	获取up向量
	 * \return	up向量
	 */
	Vector3 Transform::GetUp()
	{
		auto up = GetRotation() * Vector3::Up;
		up.Normalize();
		return up;
	}

	/**
	 * \brief	设置位置
	 * \param	position		位置
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
	 * \brief	设置位置
	 * \param	x				x坐标
	 * \param	y				y坐标
	 * \param	z				z坐标
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
	 * \brief	设置旋转
	 * \param	rotation		旋转
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
	 * \brief	设置局部坐标下的位置
	 * \param	localPosition	局部坐标下的位置
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
	 * \brief	设置局部坐标下的位置
	 * \param	x				x坐标
	 * \param	y				y坐标
	 * \param	z				z坐标
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
	 * \brief	设置局部坐标下的旋转
	 * \param	localRotation	局部坐标下的旋转
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
	 * \brief	设置缩放
	 * \param	localScale		缩放
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
	 * \brief	设置parent
	 * \param	pParent			parent指针
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
	 * \brief 反序列化
	 * \param pNode 配置节点指针
	 * \return 反序列化是否成功
	 */
	bool Transform::Deserialize(const XmlNode* pNode)
	{
		SetLocalPosition(XmlGetAttributeValue<float>(pNode, "lpx"), XmlGetAttributeValue<float>(pNode, "lpy"), XmlGetAttributeValue<float>(pNode, "lpz"));
		SetLocalRotation(XmlGetAttributeValue<float>(pNode, "lrx"), XmlGetAttributeValue<float>(pNode, "lry"), XmlGetAttributeValue<float>(pNode, "lrz"), XmlGetAttributeValue<float>(pNode, "lrw"));
		SetLocalScale(XmlGetAttributeValue<float>(pNode, "lsx"), XmlGetAttributeValue<float>(pNode, "lsy"), XmlGetAttributeValue<float>(pNode, "lsz"));

		return true;
	}

	/**
	 * \brief	Transform被销毁时的回调
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
	 * \brief	刷新变换矩阵
	 */
	void Transform::RefreshMatrix()
	{
		if (IsDirty(0x20))
		{
			if (IsDirty(0x01))
			{
				//世界坐标系下的位置有更新，需要将世界坐标系下的位置转换成局部坐标系下的位置
				m_LocalPosition = m_Parent->GetWorldToLocalMatrix().MultiplyPoint(m_Position);

				//清除脏标记
				SetDirty(0x01, false);
			}

			if (IsDirty(0x02))
			{
				//世界坐标系下的旋转有更新，需要将世界坐标系下的旋转转换成局部坐标系下的旋转
				m_LocalRotation = m_Parent->GetRotation().Inverse() * m_Rotation;

				//清除脏标记
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

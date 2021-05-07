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
	 * \brief 反序列化
	 * \param pNode 配置节点指针
	 * \return 反序列化是否成功
	 */
	bool Transform::Deserialize(const XmlNode* pNode)
	{
		m_LocalPosition.Set(XmlGetAttributeValue<float>(pNode, "lpx"), XmlGetAttributeValue<float>(pNode, "lpy"), XmlGetAttributeValue<float>(pNode, "lpz"));
		m_LocalRotation.Set(XmlGetAttributeValue<float>(pNode, "lrx"), XmlGetAttributeValue<float>(pNode, "lry"), XmlGetAttributeValue<float>(pNode, "lrz"), XmlGetAttributeValue<float>(pNode, "lrw"));
		m_LocalScale.Set(XmlGetAttributeValue<float>(pNode, "lsx"), XmlGetAttributeValue<float>(pNode, "lsy"), XmlGetAttributeValue<float>(pNode, "lsz"));

		return true;
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
			auto& localToWorldMatrix = m_GameObjectPtr->GetParent()->GetTransform()->GetLocalToWorldMatrix();
			m_Position = localToWorldMatrix.MultiplyPoint(m_LocalPosition);

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
			auto& parentRotation = m_GameObjectPtr->GetParent()->GetTransform()->GetRotation();
			m_Rotation = parentRotation * m_LocalRotation;

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
			auto& worldToLocalMatrix = m_GameObjectPtr->GetParent()->GetTransform()->GetWorldToLocalMatrix();
			m_LocalPosition = worldToLocalMatrix.MultiplyPoint(m_Position);

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
			auto& parentRotation = m_GameObjectPtr->GetParent()->GetTransform()->GetRotation();
			m_LocalRotation = parentRotation.Inverse() * m_Rotation;

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
	* \brief	设置位置
	* \param	position		位置
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
	* \brief	设置旋转
	* \param	rotation		旋转
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
	* \brief	设置局部坐标下的位置
	* \param	localPosition	局部坐标下的位置
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
	* \brief	设置局部坐标下的旋转
	* \param	localRotation	局部坐标下的旋转
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
	* \brief	设置缩放
	* \param	localScale		缩放
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
	 * \brief 刷新变换矩阵
	 */
	void Transform::RefreshMatrix()
	{
		if (IsDirty(0x20))
		{
			if (IsDirty(0x01))
			{
				//世界坐标系下的位置有更新，需要将世界坐标系下的位置转换成局部坐标系下的位置
				auto& worldToLocalMatrix = m_GameObjectPtr->GetParent()->GetTransform()->GetWorldToLocalMatrix();
				m_LocalPosition = worldToLocalMatrix.MultiplyPoint(m_Position);

				//清除脏标记
				SetDirty(0x01, false);
			}

			if (IsDirty(0x02))
			{
				//世界坐标系下的旋转有更新，需要将世界坐标系下的旋转转换成局部坐标系下的旋转
				auto& parentRotation = m_GameObjectPtr->GetParent()->GetTransform()->GetRotation();
				m_LocalRotation = parentRotation.Inverse() * m_Rotation;

				//清除脏标记
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

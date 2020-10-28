#include "GameObject/Transform.h"
#include "GameObject.h"

namespace SaplingEngine
{
	Transform::Transform() = default;

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
	 * \brief ˢ�±任����
	 */
	void Transform::RefreshMatrix()
	{
		if (m_IsLocalDataDirty)
		{
			m_LocalMatrix = Matrix4x4::Scale(m_LocalScale) * Matrix4x4::Rotate(m_LocalRotation) * Matrix4x4::Translate(m_LocalPosition);
			if (m_pGameObject->HasParent())
			{
				m_LocalToWorldMatrix = m_LocalMatrix * m_pGameObject->GetParent()->GetTransform()->GetLocalToWorldMatrix();
			}
			else
			{
				m_LocalToWorldMatrix = m_LocalMatrix;
			}
			m_WorldToLocalMatrix = m_LocalToWorldMatrix.Inverse();
			
			m_IsLocalDataDirty = true;
		}
	}
}

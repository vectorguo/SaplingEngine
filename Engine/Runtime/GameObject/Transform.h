#pragma once

#include "GameObject/Component.h"

namespace SaplingEngine
{
	class Transform final : public Component
	{
	public:
		Transform();

		/*
		 * ��ȡ�������
		 * ÿ����������붨��˺���
		 * GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_Transform;
		}

		/**
		 * \brief ��ȡ�ֲ����굽��������ı任����
		 * \return �ֲ����굽��������ı任����
		 */
		const Matrix4x4& GetLocalToWorldMatrix()
		{
			if (m_IsDirty)
			{
				RefreshMatrix();
			}
			return m_LocalToWorldMatrix;
		}
		
		/**
		 * \brief �����Ƿ��б仯
		 * \return �����Ƿ��б仯
		 */
		bool IsDirty() const
		{
			return m_IsDirty;
		}
		
		/**
		 * \brief �����л�
		 * \param pNode ���ýڵ�ָ��
		 * \return �����л��Ƿ�ɹ�
		 */
		bool Deserialize(const XmlNode* pNode) override;

	private:
		/**
		 * \brief ˢ�±任����
		 */
		void RefreshMatrix();

	private:
		/**
		 * \brief world position
		 */
		Vector3 m_Position;
		
		/**
		 * \brief local position
		 */
		Vector3 m_LocalPosition;

		/**
		 * \brief world rotation
		 */
		Quaternion m_Rotation;
		
		/**
		 * \brief local rotation
		 */
		Quaternion m_LocalRotation;

		/**
		 * \brief local scale
		 */
		Vector3 m_LocalScale;

		/**
		 * \brief �ֲ��任����
		 */
		Matrix4x4 m_LocalMatrix;

		/**
		 * \brief �ֲ�����������任����
		 */
		Matrix4x4 m_LocalToWorldMatrix;

		/**
		 * \brief �����Ƿ��б仯
		 */
		bool m_IsDirty;
	};

	using TransformPtr = std::shared_ptr<Transform>;
}
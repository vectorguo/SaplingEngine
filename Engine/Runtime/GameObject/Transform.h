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
		 * \brief ��ȡλ��
		 * \return λ�ã������޸ģ�
		 */
		const Vector3& GetPosition() const
		{
			return m_Position;
		}

		/**
		 * \brief ��ȡ��ת
		 * \return ��ת�������޸ģ�
		 */
		const Quaternion& GetRotation() const
		{
			return m_Rotation;
		}
		
		/**
		 * \brief ��ȡ�ֲ������µ�λ��
		 * \return �ֲ������µ�λ�ã������޸ģ�
		 */
		const Vector3& GetLocalPosition() const
		{
			return m_LocalPosition;
		}

		/**
		 * \brief ��ȡ�ֲ������µ���ת
		 * \return �ֲ������µ���ת�������޸ģ�
		 */
		const Quaternion& GetLocalRotation() const
		{
			return m_LocalRotation;
		}

		/**
		 * \brief ��ȡ����
		 * \return ���ţ������޸ģ�
		 */
		const Vector3& GetLocalScale() const
		{
			return m_LocalScale;
		}
		
		// /**
		//  * \brief ����λ��
		//  * \param position λ��
		//  */
		// void SetPosition(const Vector3& position)
		// {
		// 	m_Position = position;
		// 	m_IsWorldDataDirty = true;
		// }
		//
		// /**
		//  * \brief ������ת
		//  * \param rotation ��ת
		//  */
		// void SetRotation(const Quaternion& rotation)
		// {
		// 	m_Rotation = rotation;
		// 	m_IsWorldDataDirty = true;
		// }
		
		/**
		 * \brief ���þֲ������µ�λ��
		 * \param localPosition �ֲ������µ�λ��
		 */
		void SetLocalPosition(const Vector3& localPosition)
		{
			m_IsLocalDataDirty = true;
			m_LocalPosition = localPosition;
		}

		/**
		 * \brief ���þֲ������µ���ת
		 * \param localRotation �ֲ������µ���ת
		 */
		void SetLocalRotation(const Quaternion& localRotation)
		{
			m_IsLocalDataDirty = true;
			m_LocalRotation = localRotation;
		}

		/**
		 * \brief ��������
		 * \param localScale ����
		 */
		void SetLocalScale(const Vector3& localScale)
		{
			m_IsLocalDataDirty = true;
			m_LocalScale = localScale;
		}
		
		/**
		 * \brief ��ȡ�ֲ����굽��������ı任����
		 * \return �ֲ����굽��������ı任����
		 */
		const Matrix4x4& GetLocalToWorldMatrix()
		{
			RefreshMatrix();
			return m_LocalToWorldMatrix;
		}

		const Matrix4x4& GetWorldToLocalMatrix()
		{
			RefreshMatrix();
			return m_WorldToLocalMatrix;
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
		 * \brief ���絽�ֲ�����任����
		 */
		Matrix4x4 m_WorldToLocalMatrix;

		/**
		 * \brief �����Ƿ��б仯
		 */
		bool m_IsLocalDataDirty = true;
		bool m_IsWorldDataDirty = true;
	};

	using TransformSptr = std::shared_ptr<Transform>;
}
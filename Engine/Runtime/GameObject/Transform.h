#pragma once

#include "GameObject/Component.h"

namespace SaplingEngine
{
	class Transform final : public Component
	{
	public:
		Transform();

		/**
		 * \brief	��ȡ������͡�
		 *			ÿ����������붨��˺�����
		 *			GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component��
		 */
		static constexpr uint32_t GetStaticComponentType()
		{
			return ComponentType_Transform;
		}

		/**
		 * \brief	�����л�
		 * \param	pNode		���ýڵ�ָ��
		 * \return	�����л��Ƿ�ɹ�
		 */
		bool Deserialize(const XmlNode* pNode) override;

		/**
		 * \brief	��ȡλ��
		 * \return	λ�ã������޸ģ�
		 */
		const Vector3& GetPosition();

		/**
		 * \brief	��ȡ��ת
		 * \return	��ת�������޸ģ�
		 */
		const Quaternion& GetRotation();
		
		/**
		 * \brief	��ȡ�ֲ������µ�λ��
		 * \return	�ֲ������µ�λ�ã������޸ģ�
		 */
		const Vector3& GetLocalPosition();

		/**
		 * \brief	��ȡ�ֲ������µ���ת
		 * \return	�ֲ������µ���ת�������޸ģ�
		 */
		const Quaternion& GetLocalRotation();

		/**
		 * \brief	��ȡ����
		 * \return	���ţ������޸ģ�
		 */
		const Vector3& GetLocalScale();
		
		/**
		 * \brief	����λ��
		 * \param	position		λ��
		 */
		void SetPosition(const Vector3& position);
		
		/**
		 * \brief	������ת
		 * \param	rotation		��ת
		 */
		void SetRotation(const Quaternion& rotation);
		
		/**
		 * \brief	���þֲ������µ�λ��
		 * \param	localPosition	�ֲ������µ�λ��
		 */
		void SetLocalPosition(const Vector3& localPosition);

		/**
		 * \brief	���þֲ������µ���ת
		 * \param	localRotation	�ֲ������µ���ת
		 */
		void SetLocalRotation(const Quaternion& localRotation);

		/**
		 * \brief	��������
		 * \param	localScale		����
		 */
		void SetLocalScale(const Vector3& localScale);

		/**
		 * \brief	��ȡforward����
		 * \return	forward����
		 */
		Vector3 GetForward() const
		{
			auto forward = m_Rotation.operator*(Vector3::Forward);
			forward.Normalize();
			return forward;
		}

		/**
		 * \brief	��ȡright����
		 * \return	right����
		 */
		Vector3 GetRight() const
		{
			auto right = m_Rotation.operator*(Vector3::Right);
			right.Normalize();
			return right;
		}

		/**
		 * \brief	��ȡup����
		 * \return	up����
		 */
		Vector3 GetUp() const
		{
			auto up = m_Rotation.operator*(Vector3::Up);
			up.Normalize();
			return up;
		}
		
		/**
		 * \brief	��ȡ�ֲ����굽��������ı任����
		 * \return	�ֲ����굽��������ı任����
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

	private:
		/**
		 * \brief	��Ӧ��ǵ������Ƿ���������
		 *			0x00: ����ռ��µ�λ��
		 *			0x01: ����ռ��µ���ת
		 *			0x02: �ֲ��ռ��µ�λ��
		 *			0x04: �ֲ��ռ��µ���ת
		 *			0x08: �ֲ��ռ��µ�����
		 *			0x10: �任����
		 * \param	flag		�������
		 */
		inline bool IsDirty(uint8_t flag) const
		{
			return (m_DirtyFlag & flag) != 0;
		}

		/**
		 * \brief	���ö�Ӧ��ǵ�������������
		 *			0x00: ����ռ��µ�λ��
		 *			0x01: ����ռ��µ���ת
		 *			0x02: �ֲ��ռ��µ�λ��
		 *			0x04: �ֲ��ռ��µ���ת
		 *			0x08: �ֲ��ռ��µ�����
		 * 			0x10: �任����
		 * \param	flag		�������
		 * \param	isDirty		�Ƿ���������
		 */
		inline void SetDirty(uint8_t flag, bool isDirty)
		{
			if (isDirty)
			{
				m_DirtyFlag |= flag;
			}
			else
			{
				m_DirtyFlag &= ~flag;
			}
		}

		/**
		 * \brief	ˢ�±任����
		 */
		void RefreshMatrix();

	private:
		/**
		 * \brief	world position
		 */
		Vector3 m_Position;
		
		/**
		 * \brief	local position
		 */
		Vector3 m_LocalPosition;

		/**
		 * \brief	world rotation
		 */
		Quaternion m_Rotation;
		
		/**
		 * \brief	local rotation
		 */
		Quaternion m_LocalRotation;

		/**
		 * \brief	local scale
		 */
		Vector3 m_LocalScale;

		/**
		 * \brief	�ֲ�����������任����
		 */
		Matrix4x4 m_LocalToWorldMatrix;

		/**
		 * \brief	���絽�ֲ�����任����
		 */
		Matrix4x4 m_WorldToLocalMatrix;

		/**
		 * \brief	�����Ƿ��б仯
		 */
		uint8_t m_DirtyFlag = 0x10;
	};
}
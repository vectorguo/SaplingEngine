#pragma once

#include "Component.h"

namespace SaplingEngine
{
	class Transform final : public Component
	{
		friend class GameObject;

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
		 * \brief	��ȡforward����
		 * \return	forward����
		 */
		Vector3 GetForward();

		/**
		 * \brief	��ȡright����
		 * \return	right����
		 */
		Vector3 GetRight();

		/**
		 * \brief	��ȡup����
		 * \return	up����
		 */
		Vector3 GetUp();
		
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

		/**
		 * \brief	����λ��
		 * \param	position		λ��
		 */
		void SetPosition(const Vector3& position);

		/**
		 * \brief	����λ��
		 * \param	x				x����
		 * \param	y				y����
		 * \param	z				z����
		 */
		void SetPosition(float x, float y, float z);

		/**
		 * \brief	������ת
		 * \param	rotation		��ת
		 */
		void SetRotation(const Quaternion& rotation);

		/**
		 * \brief	������ת��ŷ����
		 * \param	eulerAngles		ŷ����
		 */
		void SetEulerAngles(const Vector3& eulerAngles);

		/**
		 * \brief	������ת��ŷ����
		 */
		void SetEulerAngles(float x, float y, float z);

		/**
		 * \brief	���þֲ������µ�λ��
		 * \param	localPosition	�ֲ������µ�λ��
		 */
		void SetLocalPosition(const Vector3& localPosition);

		/**
		 * \brief	���þֲ������µ�λ��
		 * \param	x				x����
		 * \param	y				y����
		 * \param	z				z����
		 */
		void SetLocalPosition(float x, float y, float z);

		/**
		 * \brief	���þֲ������µ���ת
		 * \param	localRotation	�ֲ������µ���ת
		 */
		void SetLocalRotation(const Quaternion& localRotation);

		void SetLocalRotation(float x, float y, float z, float w);

		/**
		 * \brief	���þֲ���������ת��ŷ����
		 * \param	eulerAngles		ŷ����
		 */
		void SetLocalEulerAngles(const Vector3& eulerAngles);

		/**
		 * \brief	���þֲ���������ת��ŷ����
		 */
		void SetLocalEulerAngles(float x, float y, float z);

		/**
		 * \brief	��������
		 * \param	localScale		����
		 */
		void SetLocalScale(const Vector3& localScale);

		void SetLocalScale(float x, float y, float z);

		/**
		 * \brief	LookAt
		 * \param	focusPosition	LookAtĿ��λ��
		 */
		void LookAt(const Vector3 focusPosition);

		/**
		 * \brief	��ȡparentָ��
		 * \return	parentָ��
		 */
		inline Transform* GetParent() const
		{
			return m_Parent;
		}

		/**
		 * \brief	��ȡparent������ָ��
		 * \return	parent����ָ��
		 */
		inline TransformSptr GetParentSptr() const
		{
			return SharedFromThis(m_Parent);
		}

		/**
		 * \brief	�Ƿ��и��ڵ�
		 * \return	�Ƿ��и��ڵ�
		 */
		inline bool HasParent() const
		{
			return m_Parent != nullptr;
		}

		/**
		 * \brief	����parent
		 * \param	pParent			parentָ��
		 */
		void SetParent(Transform* pParent);

		/**
		 * \brief	����parent
		 * \param	parentSptr		parent����ָ��
		 */
		void SetParent(const TransformSptr& parentSptr)
		{
			SetParent(parentSptr.Get());
		}

	protected:
		/**
		 * \brief	�����л�
		 * \param	pNode		���ýڵ�ָ��
		 * \return	�����л��Ƿ�ɹ�
		 */
		bool Deserialize(const XmlNode* pNode) override;

		/**
		 * \brief	Transform������ʱ�Ļص�
		 */
		void OnDestroy() override;

	private:
		/**
		 * \brief	��Ӧ��ǵ������Ƿ���������
		 *			0x01: ����ռ��µ�λ��
		 *			0x02: ����ռ��µ���ת
		 *			0x04: �ֲ��ռ��µ�λ��
		 *			0x08: �ֲ��ռ��µ���ת
		 *			0x10: �ֲ��ռ��µ�����
		 *			0x20: �任����
		 * \param	flag		�������
		 */
		inline bool IsDirty(uint8_t flag) const
		{
			return (m_DirtyFlag & flag) != 0;
		}

		/**
		 * \brief	���ö�Ӧ��ǵ�������������
		 *			0x01: ����ռ��µ�λ��
		 *			0x02: ����ռ��µ���ת
		 *			0x04: �ֲ��ռ��µ�λ��
		 *			0x08: �ֲ��ռ��µ���ת
		 *			0x10: �ֲ��ռ��µ�����
		 * 			0x20: �任����
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

		/**
		 * \brief	����ӽڵ�
		 * \param	pChild		�ӽڵ�ָ��
		 */
		void AddChild(Transform* pChild)
		{
			m_Children.push_back(pChild);
		}

		/**
		 * \brief	ɾ���ӽڵ�
		 * \param	pChild		�ӽڵ�ָ��
		 */
		void RemoveChild(Transform* pChild)
		{
			m_Children.erase(std::find(m_Children.begin(), m_Children.end(), pChild));
		}

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
		uint8_t m_DirtyFlag = 0x20;

		/**
		 * \brief	���ڵ�
		 */
		Transform* m_Parent = nullptr;

		/**
		 * \brief	�����ӽڵ�
		 */
		std::vector<Transform*> m_Children;
	};
}
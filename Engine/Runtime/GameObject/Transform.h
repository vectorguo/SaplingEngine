#pragma once

#include "GameObject/Component.h"

namespace SaplingEngine
{
	class Transform final : public Component
	{
	public:
		Transform();

		/**
		 * \brief	获取组件类型。
		 *			每个组件都必须定义此函数。
		 *			GameObject对于每个ComponentType只能拥有一个Component。
		 */
		static constexpr uint32_t GetStaticComponentType()
		{
			return ComponentType_Transform;
		}

		/**
		 * \brief	反序列化
		 * \param	pNode		配置节点指针
		 * \return	反序列化是否成功
		 */
		bool Deserialize(const XmlNode* pNode) override;

		/**
		 * \brief	获取位置
		 * \return	位置（不可修改）
		 */
		const Vector3& GetPosition();

		/**
		 * \brief	获取旋转
		 * \return	旋转（不可修改）
		 */
		const Quaternion& GetRotation();
		
		/**
		 * \brief	获取局部坐标下的位置
		 * \return	局部坐标下的位置（不可修改）
		 */
		const Vector3& GetLocalPosition();

		/**
		 * \brief	获取局部坐标下的旋转
		 * \return	局部坐标下的旋转（不可修改）
		 */
		const Quaternion& GetLocalRotation();

		/**
		 * \brief	获取缩放
		 * \return	缩放（不可修改）
		 */
		const Vector3& GetLocalScale();
		
		/**
		 * \brief	设置位置
		 * \param	position		位置
		 */
		void SetPosition(const Vector3& position);
		
		/**
		 * \brief	设置旋转
		 * \param	rotation		旋转
		 */
		void SetRotation(const Quaternion& rotation);
		
		/**
		 * \brief	设置局部坐标下的位置
		 * \param	localPosition	局部坐标下的位置
		 */
		void SetLocalPosition(const Vector3& localPosition);

		/**
		 * \brief	设置局部坐标下的旋转
		 * \param	localRotation	局部坐标下的旋转
		 */
		void SetLocalRotation(const Quaternion& localRotation);

		/**
		 * \brief	设置缩放
		 * \param	localScale		缩放
		 */
		void SetLocalScale(const Vector3& localScale);

		/**
		 * \brief	获取forward向量
		 * \return	forward向量
		 */
		Vector3 GetForward() const
		{
			auto forward = m_Rotation.operator*(Vector3::Forward);
			forward.Normalize();
			return forward;
		}

		/**
		 * \brief	获取right向量
		 * \return	right向量
		 */
		Vector3 GetRight() const
		{
			auto right = m_Rotation.operator*(Vector3::Right);
			right.Normalize();
			return right;
		}

		/**
		 * \brief	获取up向量
		 * \return	up向量
		 */
		Vector3 GetUp() const
		{
			auto up = m_Rotation.operator*(Vector3::Up);
			up.Normalize();
			return up;
		}
		
		/**
		 * \brief	获取局部坐标到世界坐标的变换矩阵
		 * \return	局部坐标到世界坐标的变换矩阵
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
		 * \brief	对应标记的数据是否是脏数据
		 *			0x00: 世界空间下的位置
		 *			0x01: 世界空间下的旋转
		 *			0x02: 局部空间下的位置
		 *			0x04: 局部空间下的旋转
		 *			0x08: 局部空间下的缩放
		 *			0x10: 变换矩阵
		 * \param	flag		标记类型
		 */
		inline bool IsDirty(uint8_t flag) const
		{
			return (m_DirtyFlag & flag) != 0;
		}

		/**
		 * \brief	设置对应标记的数据是脏数据
		 *			0x00: 世界空间下的位置
		 *			0x01: 世界空间下的旋转
		 *			0x02: 局部空间下的位置
		 *			0x04: 局部空间下的旋转
		 *			0x08: 局部空间下的缩放
		 * 			0x10: 变换矩阵
		 * \param	flag		标记类型
		 * \param	isDirty		是否是脏数据
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
		 * \brief	刷新变换矩阵
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
		 * \brief	局部到世界坐标变换矩阵
		 */
		Matrix4x4 m_LocalToWorldMatrix;

		/**
		 * \brief	世界到局部坐标变换矩阵
		 */
		Matrix4x4 m_WorldToLocalMatrix;

		/**
		 * \brief	数据是否有变化
		 */
		uint8_t m_DirtyFlag = 0x10;
	};
}
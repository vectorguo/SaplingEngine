#pragma once

#include "GameObject/Component.h"

namespace SaplingEngine
{
	class Transform final : public Component
	{
	public:
		Transform();

		/*
		 * 获取组件类型
		 * 每个组件都必须定义此函数
		 * GameObject对于每个ComponentType只能拥有一个Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_Transform;
		}

		/**
		 * \brief 获取位置
		 * \return 位置（不可修改）
		 */
		const Vector3& GetPosition() const
		{
			return m_Position;
		}

		/**
		 * \brief 获取旋转
		 * \return 旋转（不可修改）
		 */
		const Quaternion& GetRotation() const
		{
			return m_Rotation;
		}
		
		/**
		 * \brief 获取局部坐标下的位置
		 * \return 局部坐标下的位置（不可修改）
		 */
		const Vector3& GetLocalPosition() const
		{
			return m_LocalPosition;
		}

		/**
		 * \brief 获取局部坐标下的旋转
		 * \return 局部坐标下的旋转（不可修改）
		 */
		const Quaternion& GetLocalRotation() const
		{
			return m_LocalRotation;
		}

		/**
		 * \brief 获取缩放
		 * \return 缩放（不可修改）
		 */
		const Vector3& GetLocalScale() const
		{
			return m_LocalScale;
		}
		
		// /**
		//  * \brief 设置位置
		//  * \param position 位置
		//  */
		// void SetPosition(const Vector3& position)
		// {
		// 	m_Position = position;
		// 	m_IsWorldDataDirty = true;
		// }
		//
		// /**
		//  * \brief 设置旋转
		//  * \param rotation 旋转
		//  */
		// void SetRotation(const Quaternion& rotation)
		// {
		// 	m_Rotation = rotation;
		// 	m_IsWorldDataDirty = true;
		// }
		
		/**
		 * \brief 设置局部坐标下的位置
		 * \param localPosition 局部坐标下的位置
		 */
		void SetLocalPosition(const Vector3& localPosition)
		{
			m_IsLocalDataDirty = true;
			m_LocalPosition = localPosition;
		}

		/**
		 * \brief 设置局部坐标下的旋转
		 * \param localRotation 局部坐标下的旋转
		 */
		void SetLocalRotation(const Quaternion& localRotation)
		{
			m_IsLocalDataDirty = true;
			m_LocalRotation = localRotation;
		}

		/**
		 * \brief 设置缩放
		 * \param localScale 缩放
		 */
		void SetLocalScale(const Vector3& localScale)
		{
			m_IsLocalDataDirty = true;
			m_LocalScale = localScale;
		}
		
		/**
		 * \brief 获取局部坐标到世界坐标的变换矩阵
		 * \return 局部坐标到世界坐标的变换矩阵
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
		 * \brief 反序列化
		 * \param pNode 配置节点指针
		 * \return 反序列化是否成功
		 */
		bool Deserialize(const XmlNode* pNode) override;

	private:
		/**
		 * \brief 刷新变换矩阵
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
		 * \brief 局部变换矩阵
		 */
		Matrix4x4 m_LocalMatrix;

		/**
		 * \brief 局部到世界坐标变换矩阵
		 */
		Matrix4x4 m_LocalToWorldMatrix;

		/**
		 * \brief 世界到局部坐标变换矩阵
		 */
		Matrix4x4 m_WorldToLocalMatrix;

		/**
		 * \brief 数据是否有变化
		 */
		bool m_IsLocalDataDirty = true;
		bool m_IsWorldDataDirty = true;
	};

	using TransformSptr = std::shared_ptr<Transform>;
}
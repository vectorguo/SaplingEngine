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
		 * \brief 获取局部坐标到世界坐标的变换矩阵
		 * \return 局部坐标到世界坐标的变换矩阵
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
		 * \brief 数据是否有变化
		 * \return 数据是否有变化
		 */
		bool IsDirty() const
		{
			return m_IsDirty;
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
		 * \brief 数据是否有变化
		 */
		bool m_IsDirty;
	};

	using TransformPtr = std::shared_ptr<Transform>;
}
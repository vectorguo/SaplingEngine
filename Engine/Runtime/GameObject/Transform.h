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
		Quaternion m_LocalScale;

		/**
		 * \brief 局部变换矩阵
		 */
		Matrix4x4 m_LocalMatrix;

		/**
		 * \brief 局部到世界坐标变换矩阵
		 */
		Matrix4x4 m_LocalToWorldMatrix;
	};

	using TransformPtr = std::shared_ptr<Transform>;
}
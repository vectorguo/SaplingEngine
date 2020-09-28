#pragma once

#include "GameObject/Component.h"

namespace SaplingEngine
{
	using Math::Matrix4x4;
	using Math::Quaternion;
	using Math::Vector3;
	
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
			return 1;
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
	};

	using TransformPtr = std::shared_ptr<Transform>;
}
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
		 * ��ȡ�������
		 * ÿ����������붨��˺���
		 * GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component
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
	};

	using TransformPtr = std::shared_ptr<Transform>;
}
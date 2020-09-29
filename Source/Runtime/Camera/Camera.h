#pragma once

#include "GameObject/Component.h"

namespace SaplingEngine
{
	class Camera : public Component
	{
	public:
		/**
		 * \brief 相机投影类型
		 */
		enum class EProjection
		{
			/**
			 * \brief 正交投影
			 */
			Orthographic,
			
			/**
			 * \brief 透视投影
			 */
			Perspective,
		};
		
	public:
		Camera();

		/*
		 * 获取组件类型
		 * 每个组件都必须定义此函数
		 * GameObject对于每个ComponentType只能拥有一个Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_Camera;
		}

		/**
		 * \brief 获取相机的投影类型
		 * \return 相机的投影类型
		 */
		EProjection Projection() const
		{
			return m_Projection;
		}

		/**
		 * \brief 设置相机的投影类型
		 * \param projection 相机的投影类型
		 */
		void SetProjection(const EProjection projection)
		{
			m_Projection = projection;

			//刷新投影矩阵
			//TODO
		}

		/**
		 * \brief 获取相机fov
		 * \return fov
		 */
		float Fov() const
		{
			return m_Fov;
		}

		/**
		 * \brief 设置相机fov
		 * \param fov fov
		 */
		void SetFov(const float fov)
		{
			m_Fov = fov;

			//刷新投影矩阵
			//TODO
		}

		/**
		 * \brief 获取相机size
		 * \return size
		 */
		float Size() const
		{
			return m_Size;
		}

		/**
		 * \brief 设置相机size
		 * \param size size
		 */
		void SetSize(const float size)
		{
			m_Size = size;

			//刷新投影矩阵
			//TODO
		}

		/**
		 * \brief 获取相机近投影面
		 * \return 近投影面
		 */
		float NearClippingPlanes() const
		{
			return m_NearClippingPlanes;
		}

		/**
		 * \brief 设置相机近投影面
		 * \param nearClippingPlanes 近投影面
		 */
		void SetNearClippingPlanes(const float nearClippingPlanes)
		{
			m_NearClippingPlanes = nearClippingPlanes;

			//刷新投影矩阵
			//TODO
		}

		/**
		 * \brief 获取相机远投影面
		 * \return 远投影面
		 */
		float FarClippingPlanes() const
		{
			return m_FarClippingPlanes;
		}

		/**
		 * \brief 设置相机远投影面
		 * \param farClippingPlanes 远投影面
		 */
		void SetFarClippingPlanes(const float farClippingPlanes)
		{
			m_FarClippingPlanes = farClippingPlanes;

			//刷新投影矩阵
			//TODO
		}

		/**
		 * \brief 获取相机优先级
		 * \return 优先级
		 */
		int32_t Priority() const
		{
			return m_Priority;
		}

		/**
		 * \brief 设置相机优先级
		 * \param priority 优先级
		 */
		void SetPriority(const int priority)
		{
			m_Priority = priority;

			//刷新CameraManager中的相机排序
			//TODO
		}

		/**
		 * \brief 获取相机剔除标记
		 * \return 剔除标记
		 */
		int32_t CullingMask() const
		{
			return m_CullingMask;
		}

	private:
		void Awake() override;
		void OnDestroy() override;

	private:
		EProjection m_Projection = EProjection::Perspective;
		float m_Fov = 60.0f;
		float m_Size = 5.0f;
		float m_NearClippingPlanes = 0.3f;
		float m_FarClippingPlanes = 100.0f;

		/**
		 * \brief 相机渲染优先级
		 */
		int32_t m_Priority = 0;

		/**
		 * \brief 剔除标记
		 */
		int32_t m_CullingMask = 0xffffffff;
	};
}
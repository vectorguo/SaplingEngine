#pragma once

#include "GameObject/Component.h"
#include "GameObject/GameObject.h"

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
		Camera() = default;

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
		EProjection GetProjection() const
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
			m_IsDirty = true;
		}

		/**
		 * \brief 获取相机fov
		 * \return fov
		 */
		float GetFov() const
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
			m_IsDirty = true;
		}
		
		/**
		 * \brief 获取相机近投影面
		 * \return 近投影面
		 */
		float GetNearClippingPlanes() const
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
			m_IsDirty = true;
		}

		/**
		 * \brief 获取相机远投影面
		 * \return 远投影面
		 */
		float GetFarClippingPlanes() const
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
			m_IsDirty = true;
		}

		/**
		 * \brief 设置窗口的宽高比
		 * \param width 屏幕宽度
		 * \param height 屏幕高度
		 */
		void SetWindowSize(const uint32_t width, const uint32_t height)
		{
			m_WindowWidth = static_cast<float>(width);
			m_WindowHeight = static_cast<float>(height);
			m_IsDirty = true;
		}

		/**
		 * \brief 获取视图矩阵
		 * \return 投影视图
		 */
		Matrix4x4& GetWorldToViewMatrix()
		{
			RefreshMatrix();
			return m_WorldToViewMatrix;
		}
		
		/**
		 * \brief 获取投影矩阵
		 * \return 投影矩阵
		 */
		Matrix4x4& GetViewToProjMatrix()
		{
			RefreshMatrix();
			return m_ViewToProjMatrix;
		}

		/**
		 * \brief 获取相机优先级
		 * \return 优先级
		 */
		int32_t GetPriority() const
		{
			return m_Priority;
		}

		/**
		 * \brief 设置相机优先级
		 * \param priority 优先级
		 */
		void SetPriority(int priority);

		/**
		 * \brief 获取相机剔除标记
		 * \return 剔除标记
		 */
		int32_t GetCullingMask() const
		{
			return m_CullingMask;
		}

		/**
		 * \brief 反序列化
		 * \param pNode 配置节点指针
		 * \return 反序列化是否成功
		 */
		bool Deserialize(const XmlNode* pNode) override;

	private:
		void Awake() override;
		void OnDestroy() override;

		/**
		 * \brief 刷新矩阵
		 */
		void RefreshMatrix();

	private:
		EProjection m_Projection = EProjection::Perspective;
		float m_Fov = 60.0f;
		float m_WindowWidth = 5.0f;
		float m_WindowHeight = 5.0f;
		float m_NearClippingPlanes = 0.3f;
		float m_FarClippingPlanes = 100.0f;
		bool m_IsDirty = true;

		/**
		 * \brief 世界坐标到视图坐标的变换矩阵
		 */
		Matrix4x4 m_WorldToViewMatrix;
		
		/**
		 * \brief 视图坐标到投影坐标的变换矩阵
		 */
		Matrix4x4 m_ViewToProjMatrix;
		
		/**
		 * \brief 相机渲染优先级
		 */
		int32_t m_Priority = 0;

		/**
		 * \brief 剔除标记
		 */
		int32_t m_CullingMask = 0xffffffff;
	};

	using CameraSptr = std::shared_ptr<Camera>;
	using CameraList = std::vector<CameraSptr>;
}

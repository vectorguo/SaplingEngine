#pragma once

#include "GameObject/Component.h"
#include "GameObject/GameObject.h"

namespace SaplingEngine
{
	class Camera : public Component
	{
	public:
		/**
		 * \brief ���ͶӰ����
		 */
		enum class EProjection
		{
			/**
			 * \brief ����ͶӰ
			 */
			Orthographic,
			
			/**
			 * \brief ͸��ͶӰ
			 */
			Perspective,
		};
		
	public:
		Camera() = default;

		/*
		 * ��ȡ�������
		 * ÿ����������붨��˺���
		 * GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_Camera;
		}

		/**
		 * \brief ��ȡ�����ͶӰ����
		 * \return �����ͶӰ����
		 */
		EProjection GetProjection() const
		{
			return m_Projection;
		}

		/**
		 * \brief ���������ͶӰ����
		 * \param projection �����ͶӰ����
		 */
		void SetProjection(const EProjection projection)
		{
			m_Projection = projection;

			//ˢ��ͶӰ����
			m_IsDirty = true;
		}

		/**
		 * \brief ��ȡ���fov
		 * \return fov
		 */
		float GetFov() const
		{
			return m_Fov;
		}

		/**
		 * \brief �������fov
		 * \param fov fov
		 */
		void SetFov(const float fov)
		{
			m_Fov = fov;
			m_IsDirty = true;
		}
		
		/**
		 * \brief ��ȡ�����ͶӰ��
		 * \return ��ͶӰ��
		 */
		float GetNearClippingPlanes() const
		{
			return m_NearClippingPlanes;
		}

		/**
		 * \brief ���������ͶӰ��
		 * \param nearClippingPlanes ��ͶӰ��
		 */
		void SetNearClippingPlanes(const float nearClippingPlanes)
		{
			m_NearClippingPlanes = nearClippingPlanes;
			m_IsDirty = true;
		}

		/**
		 * \brief ��ȡ���ԶͶӰ��
		 * \return ԶͶӰ��
		 */
		float GetFarClippingPlanes() const
		{
			return m_FarClippingPlanes;
		}

		/**
		 * \brief �������ԶͶӰ��
		 * \param farClippingPlanes ԶͶӰ��
		 */
		void SetFarClippingPlanes(const float farClippingPlanes)
		{
			m_FarClippingPlanes = farClippingPlanes;
			m_IsDirty = true;
		}

		/**
		 * \brief ���ô��ڵĿ�߱�
		 * \param width ��Ļ���
		 * \param height ��Ļ�߶�
		 */
		void SetWindowSize(const uint32_t width, const uint32_t height)
		{
			m_WindowWidth = static_cast<float>(width);
			m_WindowHeight = static_cast<float>(height);
			m_IsDirty = true;
		}

		/**
		 * \brief ��ȡ��ͼ����
		 * \return ͶӰ��ͼ
		 */
		Matrix4x4& GetWorldToViewMatrix()
		{
			RefreshMatrix();
			return m_WorldToViewMatrix;
		}
		
		/**
		 * \brief ��ȡͶӰ����
		 * \return ͶӰ����
		 */
		Matrix4x4& GetViewToProjMatrix()
		{
			RefreshMatrix();
			return m_ViewToProjMatrix;
		}

		/**
		 * \brief ��ȡ������ȼ�
		 * \return ���ȼ�
		 */
		int32_t GetPriority() const
		{
			return m_Priority;
		}

		/**
		 * \brief ����������ȼ�
		 * \param priority ���ȼ�
		 */
		void SetPriority(int priority);

		/**
		 * \brief ��ȡ����޳����
		 * \return �޳����
		 */
		int32_t GetCullingMask() const
		{
			return m_CullingMask;
		}

		/**
		 * \brief �����л�
		 * \param pNode ���ýڵ�ָ��
		 * \return �����л��Ƿ�ɹ�
		 */
		bool Deserialize(const XmlNode* pNode) override;

	private:
		void Awake() override;
		void OnDestroy() override;

		/**
		 * \brief ˢ�¾���
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
		 * \brief �������굽��ͼ����ı任����
		 */
		Matrix4x4 m_WorldToViewMatrix;
		
		/**
		 * \brief ��ͼ���굽ͶӰ����ı任����
		 */
		Matrix4x4 m_ViewToProjMatrix;
		
		/**
		 * \brief �����Ⱦ���ȼ�
		 */
		int32_t m_Priority = 0;

		/**
		 * \brief �޳����
		 */
		int32_t m_CullingMask = 0xffffffff;
	};

	using CameraSptr = std::shared_ptr<Camera>;
	using CameraList = std::vector<CameraSptr>;
}

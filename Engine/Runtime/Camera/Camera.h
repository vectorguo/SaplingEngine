#pragma once

#include "GameObject/Component.h"

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
			//TODO
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

			//ˢ��ͶӰ����
			//TODO
		}

		/**
		 * \brief ��ȡ���size
		 * \return size
		 */
		float GetSize() const
		{
			return m_Size;
		}

		/**
		 * \brief �������size
		 * \param size size
		 */
		void SetSize(const float size)
		{
			m_Size = size;

			//ˢ��ͶӰ����
			//TODO
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

			//ˢ��ͶӰ����
			//TODO
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

			//ˢ��ͶӰ����
			//TODO
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

	private:
		EProjection m_Projection = EProjection::Perspective;
		float m_Fov = 60.0f;
		float m_Size = 5.0f;
		float m_NearClippingPlanes = 0.3f;
		float m_FarClippingPlanes = 100.0f;

		/**
		 * \brief �����Ⱦ���ȼ�
		 */
		int32_t m_Priority = 0;

		/**
		 * \brief �޳����
		 */
		int32_t m_CullingMask = 0xffffffff;
	};

	using CameraPtr = std::shared_ptr<Camera>;
	using CameraList = std::vector<CameraPtr>;
}
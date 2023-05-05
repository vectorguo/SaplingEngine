#pragma once

#include "Runtime/Component/Transform.h"

namespace SaplingEngine
{
	class CinemachineVirtualCamera final : public Component
	{
	public:
		CinemachineVirtualCamera() : Component(ComponentType_CinemachineVirtualCamera)
		{
			m_RunInEditorMode = true;
		}
		~CinemachineVirtualCamera() override = default;

		/**
		 * \brief	��ֹ�������ƶ�
		 */
		CinemachineVirtualCamera(const CinemachineVirtualCamera&) = delete;
		CinemachineVirtualCamera(CinemachineVirtualCamera&&) = delete;
		CinemachineVirtualCamera& operator= (const CinemachineVirtualCamera&) = delete;
		CinemachineVirtualCamera& operator= (CinemachineVirtualCamera&&) = delete;

		/**
		 * \brief	��ȡ������͡�
		 *			ÿ����������붨��˺�����
		 *			GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component��
		 */
		static constexpr uint32_t GetStaticComponentType()
		{
			return ComponentType_CinemachineVirtualCamera;
		}

		/**
		 * \brief	��ȡ��ǰ������������ȼ�
		 * \return	������������ȼ�
		 */
		inline uint32_t GetPriority() const
		{
			return m_Priority;
		}

		/**
		 * \brief	���õ�ǰ������������ȼ�
		 * \param	priority		������������ȼ�
		 */
		inline void SetPriority(uint32_t priority)
		{
			m_Priority = priority;

			//�����������
			CinemachineBrain::SortVirtualCameras();
		}

		/**
		 * \brief	���ø���Ŀ��
		 * \param	followTarget	����Ŀ�������ָ��
		 */
		inline void SetFollowTarget(const TransformSptr& followTarget)
		{
			m_FollowTarget = followTarget;
		}

		/**
		 * \brief	���ø���Ŀ��
		 * \param	followTarget	����Ŀ�������ָ��
		 */
		inline void SetFollowTarget(TransformSptr&& followTarget)
		{
			m_FollowTarget = followTarget;
		}

		/**
		 * \brief	���ó���Ŀ��
		 * \param	lookAtTarget	����Ŀ�������ָ��
		 */
		inline void SetLookAtTarget(const TransformSptr& lookAtTarget)
		{
			m_LookAtTarget = lookAtTarget;
		}

		/**
		 * \brief	���ó���Ŀ��
		 * \param	lookAtTarget	����Ŀ�������ָ��
		 */
		inline void SetLookAtTarget(TransformSptr&& lookAtTarget)
		{
			m_LookAtTarget = lookAtTarget;
		}

		/**
		 * \brief	��ȡ����ƫ��
		 */
		inline const Vector3& GetFollowOffset() const
		{
			return m_FollowOffset;
		}

		/**
		 * \brief	���ø���ƫ��
		 * \param	offset			����ƫ��
		 */
		inline void SetFollowOffset(const Vector3& offset)
		{
			m_FollowOffset = offset;
		}

		/**
		 * \brief	���ø���ƫ��
		 * \param	x				x��ƫ��
		 * \param	y				y��ƫ��
		 * \param	z				z��ƫ��
		 */
		inline void SetFollowOffset(float x, float y, float z)
		{
			m_FollowOffset.Set(x, y, z);
		}

	protected:
		/**
		 * \brief	Start
		 */
		void Start() override;

		/**
		 * \brief	����
		 */
		void Update() override;

		/**
		 * \brief	����
		 */
		void OnDestroy() override;

	private:
		/**
		 * \brief	���ȼ�
		 */
		uint32_t m_Priority = 0;

		/**
		 * \brief	����Ŀ��
		 */
		TransformSptr m_FollowTarget;

		/**
		 * \brief	����Ŀ��
		 */
		TransformSptr m_LookAtTarget;

		/**
		 * \brief	����ƫ��
		 */
		Vector3 m_FollowOffset;
	};
}
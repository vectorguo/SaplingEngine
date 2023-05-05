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
		 * \brief	禁止拷贝和移动
		 */
		CinemachineVirtualCamera(const CinemachineVirtualCamera&) = delete;
		CinemachineVirtualCamera(CinemachineVirtualCamera&&) = delete;
		CinemachineVirtualCamera& operator= (const CinemachineVirtualCamera&) = delete;
		CinemachineVirtualCamera& operator= (CinemachineVirtualCamera&&) = delete;

		/**
		 * \brief	获取组件类型。
		 *			每个组件都必须定义此函数。
		 *			GameObject对于每个ComponentType只能拥有一个Component。
		 */
		static constexpr uint32_t GetStaticComponentType()
		{
			return ComponentType_CinemachineVirtualCamera;
		}

		/**
		 * \brief	获取当前虚拟相机的优先级
		 * \return	虚拟相机的优先级
		 */
		inline uint32_t GetPriority() const
		{
			return m_Priority;
		}

		/**
		 * \brief	设置当前虚拟相机的优先级
		 * \param	priority		虚拟相机的优先级
		 */
		inline void SetPriority(uint32_t priority)
		{
			m_Priority = priority;

			//虚拟相机排序
			CinemachineBrain::SortVirtualCameras();
		}

		/**
		 * \brief	设置跟随目标
		 * \param	followTarget	跟随目标的智能指针
		 */
		inline void SetFollowTarget(const TransformSptr& followTarget)
		{
			m_FollowTarget = followTarget;
		}

		/**
		 * \brief	设置跟随目标
		 * \param	followTarget	跟随目标的智能指针
		 */
		inline void SetFollowTarget(TransformSptr&& followTarget)
		{
			m_FollowTarget = followTarget;
		}

		/**
		 * \brief	设置朝向目标
		 * \param	lookAtTarget	朝向目标的智能指针
		 */
		inline void SetLookAtTarget(const TransformSptr& lookAtTarget)
		{
			m_LookAtTarget = lookAtTarget;
		}

		/**
		 * \brief	设置朝向目标
		 * \param	lookAtTarget	朝向目标的智能指针
		 */
		inline void SetLookAtTarget(TransformSptr&& lookAtTarget)
		{
			m_LookAtTarget = lookAtTarget;
		}

		/**
		 * \brief	获取跟随偏移
		 */
		inline const Vector3& GetFollowOffset() const
		{
			return m_FollowOffset;
		}

		/**
		 * \brief	设置跟随偏移
		 * \param	offset			跟随偏移
		 */
		inline void SetFollowOffset(const Vector3& offset)
		{
			m_FollowOffset = offset;
		}

		/**
		 * \brief	设置跟随偏移
		 * \param	x				x轴偏移
		 * \param	y				y轴偏移
		 * \param	z				z轴偏移
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
		 * \brief	更新
		 */
		void Update() override;

		/**
		 * \brief	销毁
		 */
		void OnDestroy() override;

	private:
		/**
		 * \brief	优先级
		 */
		uint32_t m_Priority = 0;

		/**
		 * \brief	跟随目标
		 */
		TransformSptr m_FollowTarget;

		/**
		 * \brief	朝向目标
		 */
		TransformSptr m_LookAtTarget;

		/**
		 * \brief	跟随偏移
		 */
		Vector3 m_FollowOffset;
	};
}
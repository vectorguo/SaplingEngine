#pragma once

#include "Camera/Camera.h"
#include "GameObject/Component.h"

namespace SaplingEngine
{
	/**
	 * \brief	前向声明
	 */
	class CinemachineVirtualCamera;

	class CinemachineBrain final : public Component
	{
	public:
		CinemachineBrain();
		~CinemachineBrain() override = default;

		/**
		 * \brief	禁止拷贝和移动
		 */
		CinemachineBrain(const CinemachineBrain&) = delete;
		CinemachineBrain(CinemachineBrain&&) = delete;
		CinemachineBrain& operator= (const CinemachineBrain&) = delete;
		CinemachineBrain& operator= (CinemachineBrain&&) = delete;

		/**
		 * \brief	获取组件类型。
		 *			每个组件都必须定义此函数。
		 *			GameObject对于每个ComponentType只能拥有一个Component。
		 */
		static constexpr uint32_t GetStaticComponentType()
		{
			return ComponentType_CinemachineBrain;
		}

		/**
		 * \brief	添加虚拟相机
		 * \param	pVirtualCamera	虚拟相机指针
		 */
		static void AddVirtualCamera(CinemachineVirtualCamera* pVirtualCamera);

		/**
		 * \brief	删除虚拟相机
		 * \param	pVirtualCamera	虚拟相机指针
		 */
		static void RemoveVirtualCamera(CinemachineVirtualCamera* pVirtualCamera);

		/**
		 * \brief	排序虚拟相机
		 */
		static void SortVirtualCameras();

	protected:
		/**
		 * \brief	Start
		 */
		void Start() override;

		/**
		 * \brief	更新
		 */
		void Update() override;

	private:
		/**
		 * \brief	单例
		 */
		static CinemachineBrain* pInstance;

		/**
		 * \brief	虚拟相机列表
		 */
		static std::vector<CinemachineVirtualCamera*> virtualCameras;

		/**
		 * \brief	相机
		 */
		CameraSptr m_CameraSptr;

		/**
		 * \brief	当前虚拟相机
		 */
		CinemachineVirtualCamera* m_pCurrentVirtualCamera = nullptr;
	};
}
#pragma once

#include "Camera.h"

namespace SaplingEngine
{
	class CameraManager final
	{
		friend class Camera;
	public:
		CameraManager() = default;
		~CameraManager() = default;
		
		SINGLETON(CameraManager)

		/**
		 * \brief 获取所有相机
		 * \return 相机列表
		 */
		const CameraList& GetCameras() const
		{
			return m_Cameras;
		}
		
		/**
		 * \brief 销毁
		 */
		void Destroy();

		/**
		 * \brief 屏幕尺寸修改
		 */
		void OnWindowResize();
		
	private:
		void AddCamera(CameraSptr&& pCamera);
		void RemoveCamera(const CameraSptr& pCamera);
		void SortCamera();
		
	private:
		CameraList m_Cameras;
	};
}

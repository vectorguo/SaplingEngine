#pragma once

#include "Camera.h"
#include "Utility/Singleton.h"

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
		
	private:
		void AddCamera(CameraPtr&& pCamera);
		void RemoveCamera(const CameraPtr& pCamera);
		void SortCamera();
		
	private:
		CameraList m_Cameras;
	};
}

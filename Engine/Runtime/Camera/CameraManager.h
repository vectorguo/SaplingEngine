#pragma once

#include "Camera.h"

namespace SaplingEngine
{
	class CameraManager final
	{
		friend class Camera;
	
	public:
		/**
		 * \brief 获取所有相机
		 * \return 相机列表
		 */
		static const CameraList& GetCameras()
		{
			return m_Cameras;
		}
		
		/**
		 * \brief 销毁
		 */
		static void Destroy();

		/**
		 * \brief 屏幕尺寸修改
		 * \param width 变化后的窗口宽度
		 * \param height 变化后的窗口高度
		 */
		static void OnSceneResize(uint32_t width, uint32_t height);
		
	private:
		static void AddCamera(CameraSptr&& pCamera);
		static void RemoveCamera(const CameraSptr& pCamera);
		static void SortCamera();
		
	private:
		static CameraList m_Cameras;
	};
}

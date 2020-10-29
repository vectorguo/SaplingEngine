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
		 * \brief ��ȡ�������
		 * \return ����б�
		 */
		const CameraList& GetCameras() const
		{
			return m_Cameras;
		}
		
		/**
		 * \brief ����
		 */
		void Destroy();

		/**
		 * \brief ��Ļ�ߴ��޸�
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

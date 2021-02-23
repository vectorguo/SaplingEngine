#pragma once

#include "Camera.h"

namespace SaplingEngine
{
	class CameraManager final
	{
		friend class Camera;
	
	public:
		/**
		 * \brief ��ȡ�������
		 * \return ����б�
		 */
		static const CameraList& GetCameras()
		{
			return m_Cameras;
		}
		
		/**
		 * \brief ����
		 */
		static void Destroy();

		/**
		 * \brief ��Ļ�ߴ��޸�
		 * \param width �仯��Ĵ��ڿ��
		 * \param height �仯��Ĵ��ڸ߶�
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

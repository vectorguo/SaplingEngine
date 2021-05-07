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
		static const std::vector<Camera*>& GetCameras()
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
		static void AddCamera(Camera* pCamera);
		static void RemoveCamera(Camera* pCamera);
		static void SortCamera();
		
	private:
		static std::vector<Camera*> m_Cameras;
	};
}

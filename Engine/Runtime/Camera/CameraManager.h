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
		 * \brief Ïú»Ù
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

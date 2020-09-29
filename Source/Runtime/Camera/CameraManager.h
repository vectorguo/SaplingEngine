#pragma once

#include "Utility/Singleton.h"
#include "Camera.h"

namespace SaplingEngine
{
	class CameraManager final : public Singleton<CameraManager>
	{
		friend class Camera;
		
	private:
		void AddCamera(CameraPtr&& pCamera);
		void RemoveCamera(const CameraPtr& pCamera);
		void SortCamera();
		
	private:
		CameraList m_Cameras;
	};
}

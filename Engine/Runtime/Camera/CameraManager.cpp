#include "CameraManager.h"

namespace SaplingEngine
{
	void CameraManager::AddCamera(CameraPtr&& pCamera)
	{
		m_Cameras.push_back(pCamera);

		//≈≈–Ú
		SortCamera();
	}

	void CameraManager::RemoveCamera(const CameraPtr& pCamera)
	{
		for (auto iter = m_Cameras.begin(); iter != m_Cameras.end(); ++iter)
		{
			if (*iter == pCamera)
			{
				m_Cameras.erase(iter);
				break;
			}
		}
	}

	void CameraManager::SortCamera()
	{
		std::sort(m_Cameras.begin(), m_Cameras.end());
	}
}

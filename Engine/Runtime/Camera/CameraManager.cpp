#include "CameraManager.h"

namespace SaplingEngine
{
	/**
	 * \brief Ïú»Ù
	 */
	void CameraManager::Destroy()
	{
		
	}

	void CameraManager::AddCamera(CameraPtr&& pCamera)
	{
		m_Cameras.push_back(pCamera);

		//ÅÅĞò
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

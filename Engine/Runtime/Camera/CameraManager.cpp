#include "CameraManager.h"

#include "Application/GameSetting.h"

namespace SaplingEngine
{
	/**
	 * \brief Ïú»Ù
	 */
	void CameraManager::Destroy()
	{
		
	}

	/**
	 * \brief ÆÁÄ»³ß´çÐÞ¸Ä
	 */
	void CameraManager::OnWindowResize()
	{
		const auto width = GameSetting::Instance()->ScreenWidth();
		const auto height = GameSetting::Instance()->ScreenHeight();
		for (auto iter = m_Cameras.begin(); iter != m_Cameras.end(); ++iter)
		{
			(*iter)->SetWindowSize(width, height);
		}
	}

	void CameraManager::AddCamera(CameraPtr&& pCamera)
	{
		m_Cameras.push_back(pCamera);

		//ÅÅÐò
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
		std::sort(m_Cameras.begin(), m_Cameras.end(), [](const CameraPtr& c1, const CameraPtr& c2)
			{
				return c1->GetPriority() < c2->GetPriority();
			});
	}
}

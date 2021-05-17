#include "CameraManager.h"

#include "Application/Setting.h"

namespace SaplingEngine
{
	//静态成员初始化
	std::vector<Camera*> CameraManager::m_Cameras;
	
	/**
	 * \brief 销毁
	 */
	void CameraManager::Destroy()
	{
		m_Cameras.clear();
	}

	/**
	 * \brief 屏幕尺寸修改
	 * \param width 变化后的窗口宽度
	 * \param height 变化后的窗口高度
	 */
	void CameraManager::OnSceneResize(uint32_t width, uint32_t height)
	{
		for (auto iter = m_Cameras.begin(); iter != m_Cameras.end(); ++iter)
		{
			(*iter)->SetWindowSize(width, height);
		}
	}

	void CameraManager::AddCamera(Camera* pCamera)
	{
		m_Cameras.push_back(pCamera);

		//排序
		SortCamera();
	}

	void CameraManager::RemoveCamera(Camera* pCamera)
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
		std::sort(m_Cameras.begin(), m_Cameras.end(), [](const Camera* pCamera1, const Camera* pCamera2)
			{
				return pCamera1->GetPriority() < pCamera2->GetPriority();
			});
	}
}

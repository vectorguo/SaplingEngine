#include "CameraManager.h"

#include "Application/Setting.h"

namespace SaplingEngine
{
	//��̬��Ա��ʼ��
	std::vector<Camera*> CameraManager::m_Cameras;
	
	/**
	 * \brief ����
	 */
	void CameraManager::Destroy()
	{
		m_Cameras.clear();
	}

	/**
	 * \brief ��Ļ�ߴ��޸�
	 * \param width �仯��Ĵ��ڿ��
	 * \param height �仯��Ĵ��ڸ߶�
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

		//����
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

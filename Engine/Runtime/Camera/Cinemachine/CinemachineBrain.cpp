#include "CinemachineBrain.h"
#include "CinemachineVirtualCamera.h"

namespace SaplingEngine
{
    CinemachineBrain*                       CinemachineBrain::pInstance = nullptr;
    std::vector<CinemachineVirtualCamera*>  CinemachineBrain::virtualCameras;

    CinemachineBrain::CinemachineBrain() : Component(ComponentType_CinemachineBrain)
    {
        if (pInstance != nullptr)
        {
            throw Exception("�ظ�����CinemachineBrain");
        }
        pInstance = this;

        if (!virtualCameras.empty())
        {
            m_pCurrentVirtualCamera = virtualCameras[0];
        }

        m_RunInEditorMode = true;
    }

    /**
     * \brief	����������
     * \param	pVirtualCamera	�������ָ��
     */
    void CinemachineBrain::AddVirtualCamera(CinemachineVirtualCamera* pVirtualCamera)
    {
        auto iter = std::find(virtualCameras.begin(), virtualCameras.end(), pVirtualCamera);
        if (iter == virtualCameras.end())
        {
            virtualCameras.emplace_back(pVirtualCamera);

            //�����������
            SortVirtualCameras();
        }
    }

    /**
     * \brief	ɾ���������
     * \param	pVirtualCamera	�������ָ��
     */
    void CinemachineBrain::RemoveVirtualCamera(CinemachineVirtualCamera* pVirtualCamera)
    {
        auto iter = std::find(virtualCameras.begin(), virtualCameras.end(), pVirtualCamera);
        if (iter != virtualCameras.end())
        {
            virtualCameras.erase(iter);

            //�����������
            SortVirtualCameras();
        }
    }

    /**
     * \brief	�����������
     */
    void CinemachineBrain::SortVirtualCameras()
    {
        std::sort(virtualCameras.begin(), virtualCameras.end(), [](const CinemachineVirtualCamera* p1, const CinemachineVirtualCamera* p2) { return p1->GetPriority() > p2->GetPriority(); });

        if (pInstance != nullptr && !virtualCameras.empty())
        {
            pInstance->m_pCurrentVirtualCamera = virtualCameras[0];
        }
    }

    void CinemachineBrain::Start()
    {
        m_CameraSptr = GetComponent<Camera>();
        if (m_CameraSptr == nullptr)
        {
            throw new Exception("CinemachineBrain�Ķ�����û��Camera���");
        }
    }
    
    void CinemachineBrain::Update()
    {
        if (m_pCurrentVirtualCamera == nullptr)
        {
            return;
        }

        //ͬ���������λ�ò�������ʵ���
        auto* pTransform = GetTransform();
        auto* ptVirtualCameraTransform = m_pCurrentVirtualCamera->GetTransform();
        pTransform->SetPosition(ptVirtualCameraTransform->GetPosition());
        pTransform->SetRotation(ptVirtualCameraTransform->GetRotation());
    }
}
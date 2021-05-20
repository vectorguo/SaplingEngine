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
            throw Exception("重复创建CinemachineBrain");
        }
        pInstance = this;

        if (!virtualCameras.empty())
        {
            m_pCurrentVirtualCamera = virtualCameras[0];
        }

        m_RunInEditorMode = true;
    }

    /**
     * \brief	添加虚拟相机
     * \param	pVirtualCamera	虚拟相机指针
     */
    void CinemachineBrain::AddVirtualCamera(CinemachineVirtualCamera* pVirtualCamera)
    {
        auto iter = std::find(virtualCameras.begin(), virtualCameras.end(), pVirtualCamera);
        if (iter == virtualCameras.end())
        {
            virtualCameras.emplace_back(pVirtualCamera);

            //排序虚拟相机
            SortVirtualCameras();
        }
    }

    /**
     * \brief	删除虚拟相机
     * \param	pVirtualCamera	虚拟相机指针
     */
    void CinemachineBrain::RemoveVirtualCamera(CinemachineVirtualCamera* pVirtualCamera)
    {
        auto iter = std::find(virtualCameras.begin(), virtualCameras.end(), pVirtualCamera);
        if (iter != virtualCameras.end())
        {
            virtualCameras.erase(iter);

            //排序虚拟相机
            SortVirtualCameras();
        }
    }

    /**
     * \brief	排序虚拟相机
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
            throw new Exception("CinemachineBrain的对象上没有Camera组件");
        }
    }
    
    void CinemachineBrain::Update()
    {
        if (m_pCurrentVirtualCamera == nullptr)
        {
            return;
        }

        //同步虚拟相机位置参数给真实相机
        auto* pTransform = GetTransform();
        auto* ptVirtualCameraTransform = m_pCurrentVirtualCamera->GetTransform();
        pTransform->SetPosition(ptVirtualCameraTransform->GetPosition());
        pTransform->SetRotation(ptVirtualCameraTransform->GetRotation());
    }
}
#include "CinemachineBrain.h"
#include "CinemachineVirtualCamera.h"

namespace SaplingEngine
{
    void CinemachineVirtualCamera::Start()
    {
        CinemachineBrain::AddVirtualCamera(this);
    }
    
    void CinemachineVirtualCamera::Update()
    {
        auto* pTransform = GetTransform();

        //����λ��
        if (m_FollowTarget != nullptr)
        {
            auto offset = m_FollowTarget->GetRight() * m_FollowOffset.x + m_FollowTarget->GetUp() * m_FollowOffset.y + m_FollowTarget->GetForward() * m_FollowOffset.z;
            pTransform->SetPosition(m_FollowTarget->GetPosition() + offset);
        }

        //���³���
        if (m_LookAtTarget != nullptr)
        {

        }
    }
    
    void CinemachineVirtualCamera::OnDestroy()
    {
        CinemachineBrain::RemoveVirtualCamera(this);
    }
}
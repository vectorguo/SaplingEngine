#include "Component.h"
#include "ComponentFactory.h"

#include "Camera/Camera.h"
#include "Camera/Cinemachine/CinemachineBrain.h"
#include "Camera/Cinemachine/CinemachineVirtualCamera.h"
#include "GameObject/Transform.h"
#include "Render/Graphics/Light.h"
#include "Render/Renderer/MeshRenderer.h"

namespace SaplingEngine
{
	//静态成员初始化
	std::function<Component* (uint32_t, int32_t)> ComponentFactory::m_CustomCreator;
	
	/**
	 * \brief 根据组件类型创建组件
	 * \param componentType 组件类型
	 * \param componentSubType 组件子类型
	 * \return 组件指针
	 */
	Component* ComponentFactory::CreateComponent(uint32_t componentType, int32_t componentSubType)
	{
		switch (componentType)
		{
			case 0:
				throw Exception("illegal component type");
			case ComponentType_Transform:
				return new Transform();
			case ComponentType_Camera:
				return new Camera();
			case ComponentType_CinemachineBrain:
				return new CinemachineBrain();
			case ComponentType_CinemachineVirtualCamera:
				return new CinemachineVirtualCamera();
			case ComponentType_Light:
				return new Light();
			case ComponentType_Renderer:
				return componentSubType == 0 ? new MeshRenderer() : nullptr;
			default:
				return m_CustomCreator(componentType, componentSubType);
		}
	}
}

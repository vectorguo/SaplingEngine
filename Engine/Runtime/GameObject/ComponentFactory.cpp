#include "Camera/Camera.h"
#include "Camera/Cinemachine/CinemachineBrain.h"
#include "Camera/Cinemachine/CinemachineVirtualCamera.h"
#include "GameObject/Component.h"
#include "GameObject/ComponentFactory.h"
#include "GameObject/Transform.h"
#include "Render/Graphics/Light.h"
#include "Render/Renderer/MeshRenderer.h"

namespace SaplingEngine
{
	/**
	 * \brief	根据组件类型创建组件
	 * \param	componentType		组件类型
	 * \param	componentSubType	组件子类型
	 * \return	创建的组件的智能指针
	 */
	ComponentSptr ComponentFactory::CreateComponent(uint32_t componentType, int32_t componentSubType)
	{
		switch (componentType)
		{
			case 0:
				throw Exception("illegal component type");
			case ComponentType_Transform:
				return MakeShared<Transform>();
			case ComponentType_Camera:
				return MakeShared<Camera>();
			case ComponentType_CinemachineBrain:
				return MakeShared<CinemachineBrain>();
			case ComponentType_CinemachineVirtualCamera:
				return MakeShared<CinemachineVirtualCamera>();
			case ComponentType_Light:
				return MakeShared<Light>();
			case ComponentType_Renderer:
				return componentSubType == 0 ? MakeShared<MeshRenderer>() : nullptr;
			default:
				return CreateCustomComponent(componentType, componentSubType);
		}
	}
}

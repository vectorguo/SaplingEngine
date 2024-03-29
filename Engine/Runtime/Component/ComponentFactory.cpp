#include "Runtime/Component/Component.h"
#include "Runtime/Component/ComponentFactory.h"
#include "Runtime/Component/Transform.h"
#include "Runtime/Component/Light/Light.h"
#include "Runtime/Component/Renderer/MeshRenderer.h"

#include "Runtime/Camera/Camera.h"
#include "Runtime/Camera/Cinemachine/CinemachineBrain.h"
#include "Runtime/Camera/Cinemachine/CinemachineVirtualCamera.h"

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
#if !SAPLING_EDITOR
			case ComponentType_CinemachineBrain:
				return MakeShared<CinemachineBrain>();
			case ComponentType_CinemachineVirtualCamera:
				return MakeShared<CinemachineVirtualCamera>();
#endif
			case ComponentType_Light:
				return MakeShared<Light>();
			case ComponentType_Renderer:
				return componentSubType == 0 ? MakeShared<MeshRenderer>() : nullptr;
			default:
#if SAPLING_EDITOR
				return nullptr;
#else
				return CreateCustomComponent(componentType, componentSubType);
#endif
		}
	}
}

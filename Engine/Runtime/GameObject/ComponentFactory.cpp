#include "Component.h"
#include "ComponentFactory.h"

#include "Camera/Camera.h"
#include "GameObject/Transform.h"
#include "RenderPipeline/Renderer/MeshRenderer.h"

namespace SaplingEngine
{
	/**
	 * \brief 根据组件类型创建组件
	 * \param componentType 组件类型
	 * \param param 额外参数
	 * \return 组件指针
	 */
	Component* ComponentFactory::CreateComponent(uint32_t componentType, int32_t param) const
	{
		switch (componentType)
		{
		case 0:
			throw Exception("illegal component type");
		case ComponentType_Camera:
			return new Camera();
		case ComponentType_Renderer:
			return new MeshRenderer();
		case ComponentType_Transform:
			return new Transform();
		default:
			return m_CustomCreator(componentType, param);
		}
	}
}

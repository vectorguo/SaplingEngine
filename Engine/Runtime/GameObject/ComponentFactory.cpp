#include "Component.h"
#include "ComponentFactory.h"

#include "Camera/Camera.h"
#include "GameObject/Transform.h"
#include "Graphics/Light.h"
#include "RenderPipeline/Renderer/MeshRenderer.h"

namespace SaplingEngine
{
	/**
	 * \brief ����������ʹ������
	 * \param componentType �������
	 * \param componentSubType ���������
	 * \return ���ָ��
	 */
	Component* ComponentFactory::CreateComponent(uint32_t componentType, int32_t componentSubType) const
	{
		switch (componentType)
		{
		case 0:
			throw Exception("illegal component type");
		case ComponentType_Transform:
			return new Transform();
		case ComponentType_Camera:
			return new Camera();
		case ComponentType_Light:
			return new Light();
		case ComponentType_Renderer:
			return componentSubType == 0 ? new MeshRenderer() : nullptr;
		default:
			return m_CustomCreator(componentType, componentSubType);
		}
	}
}

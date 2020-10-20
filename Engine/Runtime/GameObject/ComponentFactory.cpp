#include "Component.h"
#include "ComponentFactory.h"

#include "Camera/Camera.h"
#include "GameObject/Transform.h"
#include "RenderPipeline/Renderer/MeshRenderer.h"

namespace SaplingEngine
{
	/**
	 * \brief ����������ʹ������
	 * \param componentType �������
	 * \param param �������
	 * \return ���ָ��
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
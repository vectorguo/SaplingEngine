#pragma once

/*
 * Windows
 */
#include <Windows.h>
#include <windowsx.h>
#include <wrl.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

/*
 * crt
 */
#include <crtdbg.h>

#if defined(DEBUG) || defined(_DEBUG)
#define CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

/*
 * math
 */
#include "Math/Math.h"
#include "Math/Color.h"
#include "Math/Matrix4x4.h"
#include "Math/Quaternion.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

/*
 * Memory
 */
#include "Memory/SharedPtr.h"

/*
 * Utility
 */
#include "Utility/Exception.h"
#include "Utility/Log.h"
#include "Utility/Object.h"
#include "Utility/Singleton.h"
#include "Utility/Time.h"
#include "Utility/Utility.h"

namespace SaplingEngine
{
	using Math::Color;
	using Math::Matrix4x4;
	using Math::Quaternion;
	using Math::Vector2;
	using Math::Vector3;
	using Math::Vector4;

	/*
	 * Ç°ÏòÉùÃ÷
	 */
	class Component;
	class Camera;
	class CinemachineBrain;
	class CinemachineVirtualCamera;
	class Light;
	class MeshRenderer;
	class Renderer;
	class Transform;

	class GameObject;

	class IResource;
	class Material;
	class Mesh;
	class Texture2D;

	class Scene;

	using ComponentSptr					= SharedPtr<Component>;
	using CameraSptr					= SharedPtr<Camera>;
	using CinemachineVirtualCameraSptr	= SharedPtr<CinemachineVirtualCamera>;
	using TransformSptr					= SharedPtr<Transform>;

	using ObjectSptr					= SharedPtr<Object>;
	using GameObjectSptr				= SharedPtr<GameObject>;

	using IResourceSptr					= SharedPtr<IResource>;

	using MaterialSptr					= SharedPtr<Material>;
	using MeshSptr						= SharedPtr<Mesh>;
	using Texture2DSptr					= SharedPtr<Texture2D>;

	using ComponentList		= std::vector<ComponentSptr>;
	using GameObjectList	= std::vector<GameObjectSptr>;
}

#if SAPLING_EDITOR
namespace SaplingEditor
{
	class EditorApplication;
	class EditorSetting;
}
#endif
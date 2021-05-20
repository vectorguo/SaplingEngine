#pragma once

#include "Editor/Utility/EditorUtility.h"

using namespace SaplingEngine;

namespace SaplingEditor
{
	class EditorCamera
	{
	public:
		/**
		 * \brief	创建编辑器相机
		 */
		static void CreateEditorCamera(const Vector3& followOffset, const Vector3& targetPosition, const Vector3& targetEulerAngles);

	private:
		/**
		 * \brief	编辑器相机
		 */
		static CameraSptr editorCamera;

		/**
		 * \brief	编辑器虚拟相机
		 */
		static CinemachineVirtualCameraSptr editorVirtualCamera;

		/**
		 * \brief	编辑器相机对象
		 */
		static GameObjectSptr editorCamraObject;

		/**
		 * \brief	编辑器虚拟相机对象
		 */
		static GameObjectSptr editorVirtualCameraObject;

		/**
		 * \brief	编辑器虚拟相机目标对象
		 */
		static GameObjectSptr editorCamraTargetObject;
	};
}
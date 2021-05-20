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
		static void CreateEditorCamera(const Vector3& cameraPosition, const Vector3& cameraEulerAngles);

		/**
		 * \brief	移动编辑器相机
		 * \param	delta			移动距离
		 */
		static void MoveEditorCamera(const Vector3& delta);

		/**
		 * \brief	旋转编辑器相机
		 * \param	eularAngles		旋转的欧拉角度
		 */
		static void RotateEditorCamera(const Vector3& eularAngles);

		/**
		 * \brief	拉近拉远
		 */
		static void ZoomEditorCamera(float value);

	private:
		/**
		 * \brief	编辑器相机
		 */
		static CameraSptr editorCamera;

		/**
		 * \brief	编辑器相机对象
		 */
		static GameObjectSptr editorCamraObject;
	};
}
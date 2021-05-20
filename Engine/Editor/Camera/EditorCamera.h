#pragma once

#include "Editor/Utility/EditorUtility.h"

using namespace SaplingEngine;

namespace SaplingEditor
{
	class EditorCamera
	{
	public:
		/**
		 * \brief	�����༭�����
		 */
		static void CreateEditorCamera(const Vector3& followOffset, const Vector3& targetPosition, const Vector3& targetEulerAngles);

	private:
		/**
		 * \brief	�༭�����
		 */
		static CameraSptr editorCamera;

		/**
		 * \brief	�༭���������
		 */
		static CinemachineVirtualCameraSptr editorVirtualCamera;

		/**
		 * \brief	�༭���������
		 */
		static GameObjectSptr editorCamraObject;

		/**
		 * \brief	�༭�������������
		 */
		static GameObjectSptr editorVirtualCameraObject;

		/**
		 * \brief	�༭���������Ŀ�����
		 */
		static GameObjectSptr editorCamraTargetObject;
	};
}
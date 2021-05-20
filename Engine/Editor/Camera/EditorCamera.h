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
		static void CreateEditorCamera(const Vector3& cameraPosition, const Vector3& cameraEulerAngles);

		/**
		 * \brief	�ƶ��༭�����
		 * \param	delta			�ƶ�����
		 */
		static void MoveEditorCamera(const Vector3& delta);

		/**
		 * \brief	��ת�༭�����
		 * \param	eularAngles		��ת��ŷ���Ƕ�
		 */
		static void RotateEditorCamera(const Vector3& eularAngles);

		/**
		 * \brief	������Զ
		 */
		static void ZoomEditorCamera(float value);

	private:
		/**
		 * \brief	�༭�����
		 */
		static CameraSptr editorCamera;

		/**
		 * \brief	�༭���������
		 */
		static GameObjectSptr editorCamraObject;
	};
}
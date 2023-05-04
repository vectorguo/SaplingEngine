#pragma once

#include "Runtime/SaplingEngineHeader.h"

#include "Game/WarcraftEnum.h"

namespace Warcraft
{
	class Character;

	class InputSystem final
	{
	public:
		/**
		 * \brief	��ʼ��
		 * \param	pPlayer		��ҽ�ɫ��ָ��
		 */
		static void Initialize(Character* pPlayer);

		/**
		 * \brief	����
		 */
		static void Update();

	private:
		/**
		 * \brief	���½�ɫģʽ
		 */
		static void UpdateCharacterMode();

		/**
		 * \brief	���±༭��ģʽ
		 */
		static void UpdateEditorMode();

	private:
		/**
		 * \brief	��ɫ�������
		 */
		static CinemachineVirtualCameraSptr characterVirtualCameraSptr;

		/**
		 * \brief	�༭���������
		 */
		static CinemachineVirtualCameraSptr editorVirtualCameraSptr;

		/**
		 * \brief	�༭��ģʽ�����������Ŀ��
		 */
		static GameObjectSptr editorTarget;

		/**
		 * \brief	�Ƿ��Ǳ༭��ģʽ
		 */
		static bool isEditorMode;
	};
}

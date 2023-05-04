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
		 * \brief	初始化
		 * \param	pPlayer		玩家角色的指针
		 */
		static void Initialize(Character* pPlayer);

		/**
		 * \brief	更新
		 */
		static void Update();

	private:
		/**
		 * \brief	更新角色模式
		 */
		static void UpdateCharacterMode();

		/**
		 * \brief	更新编辑器模式
		 */
		static void UpdateEditorMode();

	private:
		/**
		 * \brief	角色虚拟相机
		 */
		static CinemachineVirtualCameraSptr characterVirtualCameraSptr;

		/**
		 * \brief	编辑器虚拟相机
		 */
		static CinemachineVirtualCameraSptr editorVirtualCameraSptr;

		/**
		 * \brief	编辑器模式下虚拟相机的目标
		 */
		static GameObjectSptr editorTarget;

		/**
		 * \brief	是否是编辑器模式
		 */
		static bool isEditorMode;
	};
}

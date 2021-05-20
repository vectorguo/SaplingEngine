#include "Application/Setting.h"
#include "Editor/Application/EditorSetting.h"

using namespace SaplingEngine;

namespace SaplingEditor
{
	/**
	 * \brief	��ʼ��
	 * \param	width		��Ļ���
	 * \param	height		��Ļ�߶�
	 * \return	�Ƿ��ʼ���ɹ�
	 */
	void EditorSetting::Initialize(uint32_t width, uint32_t height)
	{
		Setting::Initialize();

		//���ñ༭��ģʽ
		Setting::isEditorMode = true;

		//�������ô��ڿ�Ⱥ͸߶�
		Setting::screenWidth = width;
		Setting::screenHeight = height;
	}
}
#include "Editor/Application/EditorSetting.h"

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
		//��ȡ���ڿ�Ⱥ͸߶�
		screenWidth = width;
		screenHeight = height;
	}
}
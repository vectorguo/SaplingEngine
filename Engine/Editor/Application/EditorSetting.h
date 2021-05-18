#pragma once

#include "SaplingEnginePch.h"
#include "Application/Setting.h"

namespace SaplingEditor
{
	class EditorSetting : public SaplingEngine::Setting
	{
	public:
		/**
		 * \brief	��ʼ��
		 * \param	width		��Ļ���
		 * \param	height		��Ļ�߶�
		 * \return	�Ƿ��ʼ���ɹ�
		 */
		static void Initialize(uint32_t width, uint32_t height);
	};
}
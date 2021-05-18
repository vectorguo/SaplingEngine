#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEditor
{
	class EditorSetting
	{
	public:
		/**
		 * \brief	��ʼ��
		 * \param	width		��Ļ���
		 * \param	height		��Ļ�߶�
		 * \return	�Ƿ��ʼ���ɹ�
		 */
		static void Initialize(uint32_t width, uint32_t height);

		/**
		 * \brief	��ȡ�༭��������������
		 * \return	�༭��������������
		 */
		inline static const std::string& StartSceneName()
		{
			return startSceneName;
		}

		/**
		 * \brief	��ȡ�༭����������·��
		 * \return	�༭����������·��
		 */
		inline static const std::string& StartScenePath()
		{
			return startScenePath;
		}

	private:
		/**
		 * \brief	�༭����ʼ��������
		 */
		static std::string startSceneName;

		/**
		 * \brief	�༭����ʼ����·��
		 */
		static std::string startScenePath;
	};
}
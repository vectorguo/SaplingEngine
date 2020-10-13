#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class GameSetting : public Singleton<GameSetting>
	{
	public:
		GameSetting() = default;
		~GameSetting() = default;

		GameSetting(const GameSetting&) = delete;
		GameSetting(GameSetting&&) = delete;
		GameSetting& operator=(const GameSetting&) = delete;
		GameSetting& operator=(GameSetting&&) = delete;
		
		/**
		 * \brief ��ʼ��
		 * \return �Ƿ��ʼ���ɹ�
		 */
		bool Initialize();

		/**
		 * \brief ��ȡ��Ļ���
		 * \return ��Ļ���
		 */
		uint32_t ScreenWidth() const
		{
			return m_ScreenWidth;
		}

		/**
		 * \brief ��ȡ��Ļ�߶�
		 * \return ��Ļ�߶�
		 */
		uint32_t ScreenHeight() const
		{
			return m_ScreenHeight;
		}

		/**
		 * \brief ������Ļ�ߴ�
		 * \param width ��Ļ���
		 * \param height ��Ļ�߶�
		 */
		void SetScreenSize(uint32_t width, uint32_t height)
		{
			m_ScreenWidth = width;
			m_ScreenHeight = height;
		}

	private:
		/**
		 * \brief Xml������Ϣ
		 */
		XmlDocument m_SettingDocument;
		
		/**
		 * \brief ��Ļ���
		 */
		uint32_t m_ScreenWidth = 0;
		
		/**
		 * \brief ��Ļ�߶�
		 */
		uint32_t m_ScreenHeight = 0;

		/**
		 * \brief ��ʼ����·��
		 */
		std::string m_StartScenePath;
	};
}
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
		 * \brief 初始化
		 * \return 是否初始化成功
		 */
		bool Initialize();

		/**
		 * \brief 获取屏幕宽度
		 * \return 屏幕宽度
		 */
		uint32_t ScreenWidth() const
		{
			return m_ScreenWidth;
		}

		/**
		 * \brief 获取屏幕高度
		 * \return 屏幕高度
		 */
		uint32_t ScreenHeight() const
		{
			return m_ScreenHeight;
		}

		/**
		 * \brief 设置屏幕尺寸
		 * \param width 屏幕宽度
		 * \param height 屏幕高度
		 */
		void SetScreenSize(uint32_t width, uint32_t height)
		{
			m_ScreenWidth = width;
			m_ScreenHeight = height;
		}

	private:
		/**
		 * \brief Xml配置信息
		 */
		XmlDocument m_SettingDocument;
		
		/**
		 * \brief 屏幕宽度
		 */
		uint32_t m_ScreenWidth = 0;
		
		/**
		 * \brief 屏幕高度
		 */
		uint32_t m_ScreenHeight = 0;

		/**
		 * \brief 起始场景路径
		 */
		std::string m_StartScenePath;
	};
}
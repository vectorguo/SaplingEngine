#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class GameSetting final
	{
	public:
		GameSetting() = default;
		~GameSetting();

		SINGLETON(GameSetting)
		
		/**
		 * \brief 初始化
		 * \return 是否初始化成功
		 */
		bool Initialize();

		/**
		 * \brief 获取XML配置根节点
		 * \return XML配置的根节点
		 */
		const XmlNode* GetRootNode() const
		{
			return m_pRootNode;
		}

		/**
		 * \brief 获取节点
		 * \param nodeName 节点名称
		 * \return 节点指针
		 */
		const XmlNode* GetNode(const char* nodeName) const
		{
			return m_pRootNode->first_node(nodeName);
		}

		/**
		 * \brief 获取节点
		 * \param nodeName 节点名称
		 * \return 节点指针
		 */
		const XmlNode* GetNode(const std::string& nodeName) const
		{
			return m_pRootNode->first_node(nodeName.data());
		}
		
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

		/**
		 * \brief 获取启动场景名称
		 * \return 启动场景名称
		 */
		const std::string& StartSceneName() const
		{
			return m_StartSceneName;
		}
		
		/**
		 * \brief 获取启动场景路径
		 * \return 启动场景路径
		 */
		const std::string& StartScenePath() const
		{
			return m_StartScenePath;
		}

	private:
		/**
		 * \brief Xml配置文件
		 */
		XmlDocumentFile* m_pDocumentFile = nullptr;
		
		/**
		 * \brief Xml配置信息
		 */
		XmlDocument* m_pDocument = nullptr;

		/**
		 * \brief 设置配置的根节点
		 */
		XmlNode* m_pRootNode = nullptr;
		
		/**
		 * \brief 屏幕宽度
		 */
		uint32_t m_ScreenWidth = 0;
		
		/**
		 * \brief 屏幕高度
		 */
		uint32_t m_ScreenHeight = 0;

		/**
		 * \brief 起始场景名称
		 */
		std::string m_StartSceneName;
		
		/**
		 * \brief 起始场景路径
		 */
		std::string m_StartScenePath;
	};
}
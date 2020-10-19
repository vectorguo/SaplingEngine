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
		 * \brief ��ʼ��
		 * \return �Ƿ��ʼ���ɹ�
		 */
		bool Initialize();

		/**
		 * \brief ��ȡXML���ø��ڵ�
		 * \return XML���õĸ��ڵ�
		 */
		const XmlNode* GetRootNode() const
		{
			return m_pRootNode;
		}

		/**
		 * \brief ��ȡ�ڵ�
		 * \param nodeName �ڵ�����
		 * \return �ڵ�ָ��
		 */
		const XmlNode* GetNode(const char* nodeName) const
		{
			return m_pRootNode->first_node(nodeName);
		}

		/**
		 * \brief ��ȡ�ڵ�
		 * \param nodeName �ڵ�����
		 * \return �ڵ�ָ��
		 */
		const XmlNode* GetNode(const std::string& nodeName) const
		{
			return m_pRootNode->first_node(nodeName.data());
		}
		
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

		/**
		 * \brief ��ȡ������������
		 * \return ������������
		 */
		const std::string& StartSceneName() const
		{
			return m_StartSceneName;
		}
		
		/**
		 * \brief ��ȡ��������·��
		 * \return ��������·��
		 */
		const std::string& StartScenePath() const
		{
			return m_StartScenePath;
		}

	private:
		/**
		 * \brief Xml�����ļ�
		 */
		XmlDocumentFile* m_pDocumentFile = nullptr;
		
		/**
		 * \brief Xml������Ϣ
		 */
		XmlDocument* m_pDocument = nullptr;

		/**
		 * \brief �������õĸ��ڵ�
		 */
		XmlNode* m_pRootNode = nullptr;
		
		/**
		 * \brief ��Ļ���
		 */
		uint32_t m_ScreenWidth = 0;
		
		/**
		 * \brief ��Ļ�߶�
		 */
		uint32_t m_ScreenHeight = 0;

		/**
		 * \brief ��ʼ��������
		 */
		std::string m_StartSceneName;
		
		/**
		 * \brief ��ʼ����·��
		 */
		std::string m_StartScenePath;
	};
}
#pragma once

#include "SaplingEnginePch.h"
#include "Camera/CameraManager.h"
#include "Render/RenderPipeline/RenderPipeline.h"

namespace SaplingEngine
{
	class GameSetting final
	{
	public:
		/**
		 * \brief ��ʼ��
		 * \return �Ƿ��ʼ���ɹ�
		 */
		static bool Initialize();

		/**
		 * \brief ��ȡ��Ļ���
		 * \return ��Ļ���
		 */
		static uint32_t ScreenWidth()
		{
			return m_ScreenWidth;
		}

		/**
		 * \brief ��ȡ��Ļ�߶�
		 * \return ��Ļ�߶�
		 */
		static uint32_t ScreenHeight()
		{
			return m_ScreenHeight;
		}

		/**
		 * \brief ������Ļ�ߴ�
		 * \param width ��Ļ���
		 * \param height ��Ļ�߶�
		 */
		static void SetScreenSize(uint32_t width, uint32_t height)
		{
			m_ScreenWidth = width;
			m_ScreenHeight = height;

			//֪ͨ�������Ⱦ������Ļ��С�仯
			CameraManager::OnSceneResize(width, height);
			RenderPipeline::Instance()->OnSceneResize(width, height);
		}

		/**
		 * \brief ��ȡ������������
		 * \return ������������
		 */
		static const std::string& StartSceneName()
		{
			return m_StartSceneName;
		}
		
		/**
		 * \brief ��ȡ��������·��
		 * \return ��������·��
		 */
		static const std::string& StartScenePath()
		{
			return m_StartScenePath;
		}

	private:
		/**
		 * \brief ��Ļ���
		 */
		static uint32_t m_ScreenWidth;
		
		/**
		 * \brief ��Ļ�߶�
		 */
		static uint32_t m_ScreenHeight;

		/**
		 * \brief ��ʼ��������
		 */
		static std::string m_StartSceneName;
		
		/**
		 * \brief ��ʼ����·��
		 */
		static std::string m_StartScenePath;
	};
}

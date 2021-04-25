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
			return screenWidth;
		}

		/**
		 * \brief ��ȡ��Ļ�߶�
		 * \return ��Ļ�߶�
		 */
		static uint32_t ScreenHeight()
		{
			return screenHeight;
		}

		/**
		 * \brief ������Ļ�ߴ�
		 * \param width ��Ļ���
		 * \param height ��Ļ�߶�
		 */
		static void SetScreenSize(uint32_t width, uint32_t height)
		{
			screenWidth = width;
			screenHeight = height;

			//֪ͨ�������Ⱦ������Ļ��С�仯
			CameraManager::OnSceneResize(width, height);
			RenderPipeline::OnSceneResize(width, height);
		}

		/**
		 * \brief ��ȡ������������
		 * \return ������������
		 */
		static const std::string& StartSceneName()
		{
			return startSceneName;
		}
		
		/**
		 * \brief ��ȡ��������·��
		 * \return ��������·��
		 */
		static const std::string& StartScenePath()
		{
			return startScenePath;
		}

	private:
		/**
		 * \brief ��Ļ���
		 */
		static uint32_t screenWidth;
		
		/**
		 * \brief ��Ļ�߶�
		 */
		static uint32_t screenHeight;

		/**
		 * \brief ��ʼ��������
		 */
		static std::string startSceneName;
		
		/**
		 * \brief ��ʼ����·��
		 */
		static std::string startScenePath;
	};
}

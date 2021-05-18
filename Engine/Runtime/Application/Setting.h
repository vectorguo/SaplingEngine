#pragma once

#include "SaplingEnginePch.h"
#include "Camera/CameraManager.h"
#include "Render/RenderPipeline/RenderPipeline.h"

namespace SaplingEngine
{
	class Setting
	{
	public:
		/**
		 * \brief	��ʼ��
		 * \return	�Ƿ��ʼ���ɹ�
		 */
		static bool Initialize();

		/**
		 * \brief	��ʼ��
		 * \param	width		��Ļ���
		 * \param	height		��Ļ�߶�
		 * \return	�Ƿ��ʼ���ɹ�
		 */
		static bool Initialize(uint32_t width, uint32_t height);

		/**
		 * \brief	�Ƿ��Ǳ༭��ģʽ
		 */
		static bool IsEditorMode()
		{
			return isEditorMode;
		}

		/**
		 * \brief	��ȡ��Ļ���
		 * \return	��Ļ���
		 */
		inline static uint32_t ScreenWidth()
		{
			return screenWidth;
		}

		/**
		 * \brief	��ȡ��Ļ�߶�
		 * \return	��Ļ�߶�
		 */
		inline static uint32_t ScreenHeight()
		{
			return screenHeight;
		}

		/**
		 * \brief	������Ļ�ߴ�
		 * \param	width		��Ļ���
		 * \param	height		��Ļ�߶�
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
		inline static const std::string& StartSceneName()
		{
			return startSceneName;
		}
		
		/**
		 * \brief ��ȡ��������·��
		 * \return ��������·��
		 */
		inline static const std::string& StartScenePath()
		{
			return startScenePath;
		}

	protected:
		/**
		 * \brief �Ƿ��Ǳ༭��ģʽ
		 */
		static bool isEditorMode;

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

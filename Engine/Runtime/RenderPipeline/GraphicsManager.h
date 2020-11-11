#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Camera;
	class Scene;
	class GraphicsManager
	{
	public:
		GraphicsManager();
		virtual ~GraphicsManager() = default;

		GraphicsManager(const GraphicsManager&) = delete;
		GraphicsManager(GraphicsManager&&) = delete;
		GraphicsManager& operator=(const GraphicsManager&) = delete;
		GraphicsManager& operator=(GraphicsManager&&) = delete;

		/**
		 * \brief ��ȡ����
		 * \return ����
		 */
		static GraphicsManager* Instance()
		{
			return m_Instance;
		}

		/**
		 * \brief ��ʼ��ʼ��
		 */
		virtual void BeginInitialize(HWND hWnd, uint32_t width, uint32_t height)
		{
			
		}

		/**
		 * \brief ������ʼ��
		 */
		virtual void EndInitialize(HWND hWnd, uint32_t width, uint32_t height)
		{
			
		}
		
		/**
		 * \brief ����
		 */
		virtual void OnWindowResize(uint32_t width, uint32_t height)
		{

		}

		/**
		 * \brief ��ȡObject��������������
		 * \return ��������������
		 */
		virtual uint32_t PopObjectCbIndex() = 0;

		/**
		 * \brief �黹��������������
		 * \param index ��������������
		 */
		virtual void PushObjectCbIndex(uint32_t index) = 0;
		
		/**
		 * \brief �������峣������������
		 * \param pActiveScene ��ǰ�����
		 */
		virtual void UpdateObjectConstantBuffer(Scene* pActiveScene)
		{
			
		}
		
		/**
		 * \brief ����Pass��������������
		 * \param pCamera ��ǰ���
		 */
		virtual void UpdatePassConstantBuffer(Camera* pCamera)
		{
			
		}
		
		/**
		 * \brief ����
		 */
		virtual void Destroy()
		{

		}

	private:
		/**
		 * \brief ����
		 */
		static GraphicsManager* m_Instance;
	};
}
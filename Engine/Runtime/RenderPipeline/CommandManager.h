#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Renderer;
	
	class CommandManager
	{
	public:
		CommandManager();
		virtual ~CommandManager() = default;

		CommandManager(const CommandManager&) = delete;
		CommandManager(CommandManager&&) = delete;
		CommandManager& operator=(const CommandManager&) = delete;
		CommandManager& operator=(CommandManager&&) = delete;

		/**
		 * \brief ��ȡ����
		 * \return ����
		 */
		static CommandManager* Instance()
		{
			return m_Instance;
		}

		/**
		 * \brief ��ʼ��ʼ��
		 */
		virtual void BeginInitialize() {}

		/**
		 * \brief ������ʼ��
		 */
		virtual void EndInitialize() {}

		/**
		 * \brief ִ����Ⱦǰ��׼������
		 */
		virtual void PreRender() {}

		/**
		 * \brief ִ����Ⱦ���������
		 */
		virtual void PostRender() {}

		/**
		 * \brief ִ�л���ǰ��׼������
		 */
		virtual void PreDraw(bool clearColor, bool clearDepth, const Color& color) {}

		/**
		 * \brief ִ�л��ƺ��������
		 */
		virtual void PostDraw() {}
		
		/**
		 * \brief ����
		 */
		virtual void Destroy() {}

		/**
		 * \brief ��������
		 * \param pRenderer renderer
		 */
		virtual void DrawIndexedInstanced(const Renderer* pRenderer) = 0;

	private:
		/**
		 * \brief ����
		 */
		static CommandManager* m_Instance;
	};
}
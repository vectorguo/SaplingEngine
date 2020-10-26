#pragma once

namespace SaplingEngine
{
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
		 * \brief ����
		 */
		virtual void Destroy() {}

	private:
		/**
		 * \brief ����
		 */
		static CommandManager* m_Instance;
	};
}
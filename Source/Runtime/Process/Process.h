#pragma once

namespace SaplingEngine
{
	class Process
	{
		friend class ProcessManager;

	public:
		enum class EProcessState
		{
			Uninitialized = 0,
			Removed,

			Running,
			Paused,

			Succeeded,
			Failed,
			Aborted,
		};

		Process();
		virtual ~Process();
		
		Process(const Process&) = delete;
		Process(Process&&) = delete;
		Process& operator=(const Process&) = delete;
		Process& operator=(Process&&) = delete;

	protected:
		virtual void OnInitialize()
		{
			m_State = EProcessState::Running;
		}

		virtual void OnUpdate()
		{
			
		}

		virtual void OnSuccess()
		{
			
		}

		virtual void OnFail()
		{
			
		}

		virtual void OnAbort()
		{
			
		}
		
	private:
		EProcessState m_State;
	};
}
#pragma once

namespace SaplingEngine
{
	template <typename T>
	class Singleton
	{
	protected:
		Singleton() {}
		virtual ~Singleton() {}
		
	public:
		static T* GetInstance()
		{
			if (m_Instance == nullptr)
			{
				m_Instance = new T;
			}
			return m_Instance;
		}

		static void Release()
		{
			if (m_Instance != nullptr)
			{
				delete m_Instance;
				m_Instance = nullptr;
			}
		}

		Singleton(const Singleton&) = delete;
		Singleton(Singleton&&) = delete;
		Singleton& operator= (const Singleton&) = delete;
		Singleton& operator= (Singleton&&) = delete;

		static T* m_Instance;
	};

	template <typename T> T* Singleton<T>::m_Instance = nullptr;
}
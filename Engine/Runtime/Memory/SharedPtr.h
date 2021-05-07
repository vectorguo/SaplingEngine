#pragma once

#include <cstdint>
#include <utility>

namespace SaplingEngine
{
	template <typename T1>
	class SharedPtr
	{
		template <typename T1, class... Params>
		friend SharedPtr<T1> MakeShared(Params&&... params) noexcept;

		template <typename T1, typename T2>
		friend SharedPtr<T1> static_pointer_cast(const SharedPtr<T2>& other) noexcept;

		template <typename T1, typename T2>
		friend SharedPtr<T1> static_pointer_cast(SharedPtr<T2>&& other) noexcept;

		template <typename T1, typename T2>
		friend SharedPtr<T1> const_pointer_cast(const SharedPtr<T2>& other) noexcept;

		template <typename T1, typename T2>
		friend SharedPtr<T1> const_pointer_cast(SharedPtr<T2>&& other) noexcept;

		template <typename T1, typename T2>
		friend SharedPtr<T1> reinterpret_pointer_cast(const SharedPtr<T2>& other) noexcept;

		template <typename T1, typename T2>
		friend SharedPtr<T1> reinterpret_pointer_cast(SharedPtr<T2>&& other) noexcept;

	public:
		using ElementType = T1;

	private:
		SharedPtr(T1* pointer) noexcept :
			m_Pointer(pointer)
		{
			if (pointer != nullptr)
			{
				m_pRefCount = new int32_t;
				*m_pRefCount = 1;
			}
		}

		template <typename T2>
		SharedPtr(const SharedPtr<T2>& other, T1* pointer) noexcept
		{
			other.IncRef();

			m_Pointer = pointer;
			m_pRefCount = other.m_pRefCount;
		}

		template <typename T2>
		SharedPtr(SharedPtr<T2>&& other, T1* pointer) noexcept :
			m_Pointer(pointer),
			m_pRefCount(other.m_pRefCount)
		{
			other.m_Pointer = nullptr;
			other.m_pRefCount = nullptr;
		}

	public:
		constexpr SharedPtr() = default;
		constexpr SharedPtr(std::nullptr_t) noexcept {}

		SharedPtr(const SharedPtr& other) noexcept
		{
			other.IncRef();

			m_Pointer = other.m_Pointer;
			m_pRefCount = other.m_pRefCount;
		}

		template <typename T2>
		SharedPtr(const SharedPtr<T2>& other) noexcept
		{
			other.IncRef();

			m_Pointer = other.m_Pointer;
			m_pRefCount = other.m_pRefCount;
		}

		SharedPtr(SharedPtr&& other) noexcept :
			m_Pointer(other.m_Pointer),
			m_pRefCount(other.m_pRefCount)
		{
			other.m_Pointer = nullptr;
			other.m_pRefCount = nullptr;
		}

		template <typename T2>
		SharedPtr(SharedPtr<T2>&& other) noexcept :
			m_Pointer(other.m_Pointer),
			m_pRefCount(other.m_pRefCount)
		{
			other.m_Pointer = nullptr;
			other.m_pRefCount = nullptr;
		}

		~SharedPtr() noexcept
		{
			DecRef();
		}

		SharedPtr& operator=(const SharedPtr& other) noexcept
		{
			SharedPtr(other).Swap(*this);
			return *this;
		}

		template <typename T2>
		SharedPtr& operator=(const SharedPtr<T2>& other) noexcept
		{
			SharedPtr(other).Swap(*this);
			return *this;
		}

		SharedPtr& operator=(SharedPtr&& other) noexcept
		{
			SharedPtr(std::move(other)).Swap(*this);
			return *this;
		}

		template <typename T2>
		SharedPtr& operator=(SharedPtr<T2>&& other) noexcept
		{
			SharedPtr(std::move(other)).Swap(*this);
			return *this;
		}

		inline T1& operator*() const noexcept
		{
			return *m_Pointer;
		}

		inline T1* operator->() const noexcept
		{
			return m_Pointer;
		}

		inline explicit operator bool() const noexcept
		{
			return m_Pointer != nullptr;
		}

		inline T* Get() const noexcept
		{
			return m_Pointer;
		}

		void Reset()
		{
			DecRef();

			m_Pointer = nullptr;
			m_pRefCount = nullptr;
		}

	private:
		inline void IncRef() const
		{
			if (m_pRefCount)
			{
				++(*m_pRefCount);
			}
		}

		inline void DecRef() const
		{
			if (m_pRefCount)
			{
				if (--(*m_pRefCount) == 0)
				{
					Destroy();
				}
			}
		}

		void Destroy()
		{

		}

		void Swap(SharedPtr& other)
		{
			std::swap(m_Pointer, other.m_Pointer);
			std::swap(m_pRefCount, other.m_pRefCount);
		}

	private:
		T1* m_Pointer = nullptr;
		int32_t* m_pRefCount = nullptr;
		int32_t m_Size = 0;
	};

	template <typename T1, class... Params>
	SharedPtr<T1> MakeShared(Params&&... params) noexcept
	{

	}

	template <typename T1, typename T2>
	bool operator==(const SharedPtr<T1>& left, const SharedPtr<T2>& right) noexcept
	{
		return left.Get() == right.Get();
	}

	template <typename T1, typename T2>
	bool operator!=(const SharedPtr<T1>& left, const SharedPtr<T2>& right) noexcept
	{
		return left.Get() != right.Get();
	}

	template <typename T>
	bool operator==(const SharedPtr<T>& left, nullptr_t) noexcept
	{
		return left.Get() == nullptr;
	}

	template <typename T>
	bool operator==(nullptr_t, const SharedPtr<T>& right) noexcept
	{
		return nullptr == right.Get();
	}

	template <typename T>
	bool operator!=(const SharedPtr<T>& left, nullptr_t) noexcept
	{
		return left.Get() != nullptr;
	}

	template <typename T>
	bool operator!=(nullptr_t, const SharedPtr<T>& right) noexcept
	{
		return nullptr != right.Get();
	}

	template <typename T1, typename T2>
	SharedPtr<T1> static_pointer_cast(const SharedPtr<T2>& other) noexcept
	{
		const auto pointer = static_cast<typename SharedPtr<T1>::ElementType*>(other.Get());
		return SharedPtr<T1>(other, pointer);
	}

	template <typename T1, typename T2>
	SharedPtr<T1> static_pointer_cast(SharedPtr<T2>&& other) noexcept
	{
		const auto pointer = static_cast<typename SharedPtr<T1>::ElementType*>(other.Get());
		return SharedPtr<T1>(std::move(other), pointer);
	}

	template <typename T1, typename T2>
	SharedPtr<T1> const_pointer_cast(const SharedPtr<T2>& other) noexcept
	{
		const auto pointer = const_cast<typename SharedPtr<T1>::ElementType*>(other.Get());
		return SharedPtr<T1>(other, pointer);
	}

	template <typename T1, typename T2>
	SharedPtr<T1> const_pointer_cast(SharedPtr<T2>&& other) noexcept
	{
		const auto pointer = const_cast<typename SharedPtr<T1>::ElementType*>(other.Get());
		return SharedPtr<T1>(std::move(other), pointer);
	}

	template <typename T1, typename T2>
	SharedPtr<T1> reinterpret_pointer_cast(const SharedPtr<T2>& other) noexcept
	{
		const auto pointer = reinterpret_cast<typename SharedPtr<T1>::ElementType*>(other.Get());
		return SharedPtr<T1>(other, pointer);
	}

	template <typename T1, typename T2>
	SharedPtr<T1> reinterpret_pointer_cast(SharedPtr<T2>&& other) noexcept
	{
		const auto pointer = reinterpret_cast<typename SharedPtr<T1>::ElementType*>(other.Get());
		return SharedPtr<T1>(std::move(other), pointer);
	}
}
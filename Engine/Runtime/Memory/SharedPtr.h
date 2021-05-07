#pragma once

#include <cstdint>
#include <new>
#include <utility>

namespace SaplingEngine
{
	template <typename T1>
	class SharedPtr
	{
		template <typename T1, typename... Params>
		friend SharedPtr<T1> MakeShared(Params&&... params) noexcept;

		template <typename T1, typename T2>
		friend SharedPtr<T1> StaticPointerCast(const SharedPtr<T2>& other) noexcept;

		template <typename T1, typename T2>
		friend SharedPtr<T1> StaticPointerCast(SharedPtr<T2>&& other) noexcept;

		template <typename T1, typename T2>
		friend SharedPtr<T1> ConstPointerCast(const SharedPtr<T2>& other) noexcept;

		template <typename T1, typename T2>
		friend SharedPtr<T1> ConstPointerCast(SharedPtr<T2>&& other) noexcept;

		template <typename T1, typename T2>
		friend SharedPtr<T1> ReinterpretPointerCast(const SharedPtr<T2>& other) noexcept;

		template <typename T1, typename T2>
		friend SharedPtr<T1> ReinterpretPointerCast(SharedPtr<T2>&& other) noexcept;

	public:
		using ElementType = T1;

	private:
		SharedPtr(T1* pointer, int32_t* refCountPtr) noexcept :
			m_Pointer(pointer),
			m_RefCountPtr(refCountPtr)
		{
			IncRef();
		}

		template <typename T2>
		SharedPtr(const SharedPtr<T2>& other, T1* pointer) noexcept
		{
			other.IncRef();

			m_Pointer = pointer;
			m_RefCountPtr = other.m_RefCountPtr;
		}

		template <typename T2>
		SharedPtr(SharedPtr<T2>&& other, T1* pointer) noexcept :
			m_Pointer(pointer),
			m_RefCountPtr(other.m_RefCountPtr)
		{
			other.m_Pointer = nullptr;
			other.m_RefCountPtr = nullptr;
		}

	public:
		constexpr SharedPtr() = default;
		constexpr SharedPtr(std::nullptr_t) noexcept {}

		SharedPtr(const SharedPtr& other) noexcept
		{
			other.IncRef();

			m_Pointer = other.m_Pointer;
			m_RefCountPtr = other.m_RefCountPtr;
		}

		template <typename T2>
		SharedPtr(const SharedPtr<T2>& other) noexcept
		{
			other.IncRef();

			m_Pointer = other.m_Pointer;
			m_RefCountPtr = other.m_RefCountPtr;
		}

		SharedPtr(SharedPtr&& other) noexcept :
			m_Pointer(other.m_Pointer),
			m_RefCountPtr(other.m_RefCountPtr)
		{
			other.m_Pointer = nullptr;
			other.m_RefCountPtr = nullptr;
		}

		template <typename T2>
		SharedPtr(SharedPtr<T2>&& other) noexcept :
			m_Pointer(other.m_Pointer),
			m_RefCountPtr(other.m_RefCountPtr)
		{
			other.m_Pointer = nullptr;
			other.m_RefCountPtr = nullptr;
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

		inline T1* Get() const noexcept
		{
			return m_Pointer;
		}

		void Reset()
		{
			DecRef();

			m_Pointer = nullptr;
			m_RefCountPtr = nullptr;
		}

	private:
		inline void IncRef() const
		{
			if (m_RefCountPtr)
			{
				++(*m_RefCountPtr);
			}
		}

		inline void DecRef()
		{
			if (m_RefCountPtr)
			{
				if (--(*m_RefCountPtr) == 0)
				{
					Destroy();
				}
			}
		}

		void Destroy()
		{
			m_Pointer->~T1();
			::operator delete(m_RefCountPtr);
		}

		void Swap(SharedPtr& other)
		{
			std::swap(m_Pointer, other.m_Pointer);
			std::swap(m_RefCountPtr, other.m_RefCountPtr);
		}

	private:
		T1* m_Pointer = nullptr;
		int32_t* m_RefCountPtr = nullptr;
	};

	template <typename T1, typename... Params>
	SharedPtr<T1> MakeShared(Params&&... params) noexcept
	{
		auto blockSize = sizeof(T) + sizeof(int32_t) + sizeof(int32_t);
		auto* pRefCount = static_cast<int32_t>(::operator new(blockSize));
		*pRefCount = 0;
		*(pRefCount + 1) = blockSize;
		auto* pointer = ::new (pSize + 2) T1(std::forward<Params>(params)...);
		return SharedPtr<T1>(pointer);
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
	SharedPtr<T1> StaticPointerCast(const SharedPtr<T2>& other) noexcept
	{
		const auto pointer = static_cast<typename SharedPtr<T1>::ElementType*>(other.Get());
		return SharedPtr<T1>(other, pointer);
	}

	template <typename T1, typename T2>
	SharedPtr<T1> StaticPointerCast(SharedPtr<T2>&& other) noexcept
	{
		const auto pointer = static_cast<typename SharedPtr<T1>::ElementType*>(other.Get());
		return SharedPtr<T1>(std::move(other), pointer);
	}

	template <typename T1, typename T2>
	SharedPtr<T1> ConstPointerCast(const SharedPtr<T2>& other) noexcept
	{
		const auto pointer = const_cast<typename SharedPtr<T1>::ElementType*>(other.Get());
		return SharedPtr<T1>(other, pointer);
	}

	template <typename T1, typename T2>
	SharedPtr<T1> ConstPointerCast(SharedPtr<T2>&& other) noexcept
	{
		const auto pointer = const_cast<typename SharedPtr<T1>::ElementType*>(other.Get());
		return SharedPtr<T1>(std::move(other), pointer);
	}

	template <typename T1, typename T2>
	SharedPtr<T1> ReinterpretPointerCast(const SharedPtr<T2>& other) noexcept
	{
		const auto pointer = reinterpret_cast<typename SharedPtr<T1>::ElementType*>(other.Get());
		return SharedPtr<T1>(other, pointer);
	}

	template <typename T1, typename T2>
	SharedPtr<T1> ReinterpretPointerCast(SharedPtr<T2>&& other) noexcept
	{
		const auto pointer = reinterpret_cast<typename SharedPtr<T1>::ElementType*>(other.Get());
		return SharedPtr<T1>(std::move(other), pointer);
	}

	template <typename T>
	SharedPtr<T> SharedFromThis(T* pointer)
	{
		auto* pRefCount = static_cast<int32_t*>(pointer) - 2;
		return SharedPtr<T>(pointer, pRefCount);
	}
}
#pragma once

#include <cstdint>
#include <utility>

namespace SaplingEngine
{
	template <typename T1>
	class SharedPtr
	{
		template <typename TY1, typename... Params>
		friend SharedPtr<TY1> MakeShared(Params&&... params) noexcept;

		template <typename TY1, typename TY2>
		friend SharedPtr<T1> StaticPointerCast(const SharedPtr<TY2>& other) noexcept;

		template <typename TY1, typename TY2>
		friend SharedPtr<T1> StaticPointerCast(SharedPtr<TY2>&& other) noexcept;

		template <typename TY1, typename TY2>
		friend SharedPtr<T1> ConstPointerCast(const SharedPtr<TY2>& other) noexcept;

		template <typename TY1, typename TY2>
		friend SharedPtr<T1> ConstPointerCast(SharedPtr<TY2>&& other) noexcept;

		template <typename TY1, typename TY2>
		friend SharedPtr<T1> ReinterpretPointerCast(const SharedPtr<TY2>& other) noexcept;

		template <typename TY1, typename TY2>
		friend SharedPtr<T1> ReinterpretPointerCast(SharedPtr<TY2>&& other) noexcept;

	public:
		using ElementType = T1;

	private:
		SharedPtr(T1* pointer) noexcept :
			m_Pointer(pointer),
			m_RefCountPtr(new int32_t)
		{
			*m_RefCountPtr = 1;
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
			delete m_Pointer;
			delete m_RefCountPtr;
		}

		void Swap(SharedPtr& other)
		{
			std::swap(m_Pointer, other.m_Pointer);
			std::swap(m_RefCountPtr, other.m_RefCountPtr);
		}

	private:
		T1* m_Pointer = nullptr;
		int32_t* m_RefCountPtr = nullptr;
		int32_t m_Size = 0;
	};

	template <typename TY1, typename... Params>
	SharedPtr<TY1> MakeShared(Params&&... params) noexcept
	{
		auto* pointer = new TY1(std::forward<Params>(params)...);
		return SharedPtr<TY1>(pointer);
	}

	template <typename TY1, typename TY2>
	bool operator==(const SharedPtr<TY1>& left, const SharedPtr<TY2>& right) noexcept
	{
		return left.Get() == right.Get();
	}

	template <typename TY1, typename TY2>
	bool operator!=(const SharedPtr<TY1>& left, const SharedPtr<TY2>& right) noexcept
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

	template <typename TY1, typename TY2>
	SharedPtr<TY1> StaticPointerCast(const SharedPtr<TY2>& other) noexcept
	{
		const auto pointer = static_cast<typename SharedPtr<TY1>::ElementType*>(other.Get());
		return SharedPtr<TY1>(other, pointer);
	}

	template <typename TY1, typename TY2>
	SharedPtr<TY1> StaticPointerCast(SharedPtr<TY2>&& other) noexcept
	{
		const auto pointer = static_cast<typename SharedPtr<TY1>::ElementType*>(other.Get());
		return SharedPtr<TY1>(std::move(other), pointer);
	}

	template <typename TY1, typename TY2>
	SharedPtr<TY1> ConstPointerCast(const SharedPtr<TY2>& other) noexcept
	{
		const auto pointer = const_cast<typename SharedPtr<TY1>::ElementType*>(other.Get());
		return SharedPtr<TY1>(other, pointer);
	}

	template <typename TY1, typename TY2>
	SharedPtr<TY1> ConstPointerCast(SharedPtr<TY2>&& other) noexcept
	{
		const auto pointer = const_cast<typename SharedPtr<TY1>::ElementType*>(other.Get());
		return SharedPtr<TY1>(std::move(other), pointer);
	}

	template <typename TY1, typename TY2>
	SharedPtr<TY1> ReinterpretPointerCast(const SharedPtr<TY2>& other) noexcept
	{
		const auto pointer = reinterpret_cast<typename SharedPtr<TY1>::ElementType*>(other.Get());
		return SharedPtr<TY1>(other, pointer);
	}

	template <typename TY1, typename TY2>
	SharedPtr<TY1> ReinterpretPointerCast(SharedPtr<TY2>&& other) noexcept
	{
		const auto pointer = reinterpret_cast<typename SharedPtr<TY1>::ElementType*>(other.Get());
		return SharedPtr<TY1>(std::move(other), pointer);
	}
}
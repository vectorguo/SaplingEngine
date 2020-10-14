#pragma once

namespace SaplingEngine
{
	#define SINGLETON(T) T(const T&) = delete;\
		T(T&&) = delete;\
		T& operator=(const T&) = delete;\
		T& operator=(T&&) = delete;\
		static T* Instance() { static T instance; return &instance; }
}
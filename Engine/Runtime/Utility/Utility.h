#pragma once

#include <string>

namespace SaplingEngine
{
	inline std::wstring CharToWChar(const char* c, size_t encode = CP_ACP)
	{
		const int len = MultiByteToWideChar(static_cast<uint32_t>(encode), 0, c, static_cast<int32_t>(strlen(c)), nullptr, 0);
		auto* bytes = new wchar_t[len + 1];
		MultiByteToWideChar(static_cast<uint32_t>(encode), 0, c, static_cast<int32_t>(strlen(c)), bytes, len);
		bytes[len] = '\0';
		std::wstring str = bytes;
		delete[] bytes;
		return str;
	}

	inline std::string WCharToChar(const wchar_t* wc, size_t encode = CP_ACP)
	{
		const int len = WideCharToMultiByte(static_cast<uint32_t>(encode), 0, wc, static_cast<int32_t>(wcslen(wc)), nullptr, 0, nullptr, nullptr);
		char* bytes = new char[len + 1];
		WideCharToMultiByte(static_cast<uint32_t>(encode), 0, wc, static_cast<int32_t>(wcslen(wc)), bytes, len, nullptr, nullptr);
		bytes[len] = '\0';
		std::string str = bytes;
		delete[] bytes;
		return str;
	}
}

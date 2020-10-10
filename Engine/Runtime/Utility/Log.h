#pragma once

#include <iostream>

namespace SaplingEngine
{
	class Log
	{
	public:
		static void LogInfo(const char* message)
		{
			OutputDebugStringA(message);
			OutputDebugStringA("\n");
		}

		static void LogInfo(const std::string& message)
		{
			OutputDebugStringA(message.c_str());
			OutputDebugStringA("\n");
		}

		static void LogWarning(const char* message)
		{
			OutputDebugStringA(message);
			OutputDebugStringA("\n");
		}

		static void LogWarning(const std::string& message)
		{
			OutputDebugStringA(message.c_str());
			OutputDebugStringA("\n");
		}

		static void LogError(const char* message)
		{
			OutputDebugStringA(message);
			OutputDebugStringA("\n");
		}

		static void LogError(const std::string& message)
		{
			OutputDebugStringA(message.c_str());
			OutputDebugStringA("\n");
		}
	};
}

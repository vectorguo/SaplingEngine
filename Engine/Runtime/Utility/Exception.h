#pragma once

#include <string>

class Exception : public std::exception
{
public:
	Exception(const std::string& msg) : m_Message(msg)
	{
		
	}

	Exception(std::string&& msg) : m_Message(std::move(msg))
	{

	}

	const std::string& Message() const { return m_Message; }

private:
	std::string m_Message;
};

/*
 * Òì³£º¯Êý
 */
constexpr void ThrowIfFailed(const HRESULT result)
{
	if (FAILED(result))
	{
		throw Exception("");
	}
}

constexpr void ThrowIfFailed(const HRESULT result, const std::string& message)
{
	if (FAILED(result))
	{
		throw Exception(message);
	}
}

constexpr void ThrowIfFailed(const HRESULT result, std::string&& message)
{
	if (FAILED(result))
	{
		throw Exception(std::move(message));
	}
}
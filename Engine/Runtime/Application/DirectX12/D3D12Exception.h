#pragma once

#include <string>

class D3D12Exception : public std::exception
{
public:
	D3D12Exception(const std::string& msg) : m_Message(msg)
	{
		
	}

	D3D12Exception(std::string&& msg) : m_Message(std::move(msg))
	{

	}

	const std::string& Message() const { return m_Message; }

private:
	std::string m_Message;
};

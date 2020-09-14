#pragma once

#include "SaplingEngineHeader.h"

namespace SaplingEngine
{
	class Component
	{
	public:
		Component();
		virtual ~Component();

		/*
		 * ��ֹ�������ƶ�
		 */
		Component(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator= (const Component&) = delete;
		Component& operator= (Component&&) = delete;

	private:
	};
}
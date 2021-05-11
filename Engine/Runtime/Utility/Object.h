#pragma once

namespace SaplingEngine
{
	class Object
	{
	protected:
		Object() = default;

	public:
		virtual ~Object() = default;

		/*
		 * ½ûÖ¹¿½±´ºÍÒÆ¶¯
		 */
		Object(const Object&) = delete;
		Object(Object&&) = delete;
		Object& operator= (const Object&) = delete;
		Object& operator= (Object&&) = delete;
	};
}
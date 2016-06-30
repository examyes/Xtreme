#pragma once

namespace XASM
{
	class uncopyable
	{
	protected:
		uncopyable(){};
		~uncopyable(){};

	private:
		uncopyable(const uncopyable&) = delete;
		uncopyable(const uncopyable&&) = delete;
		uncopyable& operator = (const uncopyable&) = delete;
		uncopyable& operator = (const uncopyable&&) = delete;
	};

	template <typename T>
	class Singleton : public uncopyable
	{
	public:
		static T* Instance()
		{
			static T instance_obj;
			return &instance_obj;
		}
	};

}

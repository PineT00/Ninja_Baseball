#pragma once

template <typename T>
class Singleton
{
protected :
	Singleton()						= default;
	Singleton(const Singleton& ref) = delete;
	Singleton(Singleton&& ref)		= delete;
	virtual ~Singleton()			= default;

	Singleton& operator=(const Singleton& ref) = delete;
	Singleton& operator=(Singleton&& ref)	   = delete;

public :
	static T& Instance()
	{
		static T instance;

		return instance;
	}
};
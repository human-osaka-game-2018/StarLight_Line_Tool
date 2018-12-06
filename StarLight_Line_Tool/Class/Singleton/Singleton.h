#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>

template <class T>
class Singleton
{
public:
	Singleton() {};
	virtual ~Singleton() {};

	inline static T& GetInstance()
	{
		static std::unique_ptr<T> ptr(new T());

		return *ptr;
	}
};

#endif // !SINGLETON_H

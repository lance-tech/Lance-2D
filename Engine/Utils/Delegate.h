#pragma once
#include <stdlib.h>

class Callback
{
public:
	Callback() {}
	~Callback() {}

	virtual void invoke() = 0;
};


template <typename T, typename Method>
class MethodCallback : public Callback
{
private:
	void* object_;
	Method method_;

public:
	MethodCallback(void* object, Method method)
		: object_(object), method_(method)
	{}

	virtual void invoke()
	{
		T* obj = static_cast<T*>(object_);
		return (obj->*method_)();
	}
};

class  Delegate
{
private:
	Callback* callback_;

public:
	Delegate();
	~Delegate();

	template <typename T, typename Method>
	void Connect(T* object, Method method) {
		callback_ = new MethodCallback<T, Method>(object, method);
	}

	void operator()();
	bool IsNull();
	/*bool operator==(const Delegate &other) const;
	bool operator!=(const Delegate &other) const;*/
};
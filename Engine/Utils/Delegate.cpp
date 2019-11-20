#include "Delegate.h"

Delegate::Delegate() : callback_(nullptr)
{
}

Delegate::~Delegate()
{
	delete callback_;
}

void Delegate::operator()()
{
	return callback_->invoke();
}

bool Delegate::IsNull()
{
	if (this->callback_ == nullptr)
	{
		return true;
	}
	else 
	{
		return false;
	}
}

//bool Delegate::operator==(const Delegate &other) const
//{
//	return (this->callback_ == other.callback_);
//}
//
//bool Delegate::operator!=(const Delegate &other) const
//{
//	return !(this->callback_ == other.callback_);
//}
#pragma once

#include <base/base.h>

class GLFW_ : public non_copyable
{
public:
	friend class SingletonHolder<GLFW_>;

	bool isInitialized() const;

private:

	GLFW_();
	~GLFW_();

	bool isInitialized_;
};

typedef SingletonHolder<GLFW_> GLFW;

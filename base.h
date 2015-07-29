#pragma once

#include <memory.h>
#include <assert.h>
#include <iostream>

class non_copyable {
public:
	non_copyable() = default;
	non_copyable(const non_copyable&) = delete;
	non_copyable & operator=(const non_copyable&) = delete;
};

template <class T>
class SingletonHolder : public non_copyable {
public:
	typedef T HoldedClass;

	static HoldedClass* instance() {
		assert(started());
		return unique_instance_;
	}

	static void start() {
		assert(!started());
		if (!started()) {
			unique_instance_ = new HoldedClass;
		}
	}

	template <class P0>
	static void start(P0 p0) {
		assert(!started());
		if (!started()) {
			unique_instance_ = new HoldedClass(p0);
		}
	}

	static void stop() {
		assert(started());
		delete unique_instance_;
		unique_instance_ = NULL;
	}

	static bool started() {
		return unique_instance_ != NULL;
	}

private:
	static HoldedClass *unique_instance_;
};

#define SINGLETON_HOLDER_IMPL(C) \
template<> \
C::HoldedClass * SingletonHolder<C::HoldedClass>::unique_instance_ = NULL;

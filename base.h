#pragma once

#include <memory>
#include <assert.h>
#include <iostream>

struct Rect{
	float x, y;
	float width, height;

	Rect(float x = 0,float y = 0, float width = 0, float height= 0)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	void resize(float width,float height){
		this->width = width;
		this->height = height;
	}

	void move(float dx,float dy){
		x += dx;
		y += dy;
	}

	void center(float& x, float& y){
		x = width/2 + this->x;
		y = height/2 + this->y;
	}


};

static Rect operator + (const Rect& lRect, const Rect& rRect){
	return Rect(lRect.x + rRect.x,
				lRect.y + rRect.y,
				lRect.width + rRect.width,
				lRect.height + rRect.height);
}


static std::ostream& operator << (std::ostream& stream, const Rect& rect){
	return stream << "Rect ("<<rect.x <<" "<<rect.y<<") ["<<rect.width <<" "<<rect.height<<"]";
}

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

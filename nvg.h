#pragma once

#include <base.h>

#include <nanovg.h>

class NVG_ : public non_copyable
{
public:
	friend class SingletonHolder<NVG_>;

	bool isInitialized() const;
	NVGcontext* context() const;

private:

	NVG_();
	~NVG_();

	typedef std::unique_ptr<NVGcontext,void(*)(NVGcontext*)> ScopedNVPtr;
	ScopedNVPtr vg_;

	bool isInitialized_;
};

typedef SingletonHolder<NVG_> NVG;

//overloaded functions to be used with a Rect

NVGpaint nvgBoxGradient(NVGcontext *ctx, Rect rect, float r, float f, NVGcolor icol, NVGcolor ocol);
void nvgRect(NVGcontext *ctx, Rect rect);
void nvgRoundedRect(NVGcontext *ctx, Rect rect, float r);


#include "nvg.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define NANOVG_GL2
#include <nanovg_gl.h>

bool NVG_::isInitialized() const {
	return isInitialized_;
}

NVGcontext*NVG_::context() const {
	return vg_.get();
}

NVG_::NVG_():
	isInitialized_(false),
	vg_(nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG),nvgDeleteGL2){

	if (vg_ == NULL) {
		printf("Could not init nanovg.\n");
		return;
	}
	isInitialized_ = true;
}

NVG_::~NVG_(){
}

NVGpaint nvgBoxGradient(NVGcontext* ctx, Rect rect, float r, float f, NVGcolor icol, NVGcolor ocol){
	return nvgBoxGradient(ctx,rect.x,rect.y,rect.width,rect.height,r,f,icol,ocol);
}

void nvgRect(NVGcontext* ctx, Rect rect){
	nvgRect(ctx,rect.x,rect.y,rect.width,rect.height);
}

void nvgRoundedRect(NVGcontext* ctx, Rect rect, float r){
	nvgRoundedRect(ctx,rect.x,rect.y,rect.width,rect.height,r);
}

SINGLETON_HOLDER_IMPL(NVG)

#define NANOVG_GL_IMPLEMENTATION
#include <nanovg_gl.h>


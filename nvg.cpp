#include "nvg.h"

#include <OPENGL/gl.h>

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

SINGLETON_HOLDER_IMPL(NVG)

#define NANOVG_GL_IMPLEMENTATION
#include <nanovg_gl.h>


#include "drawutils.h"

void drawShadow(NVGcontext* context, Rect rect) {

	NVGpaint shadowPaint = nvgBoxGradient(context, rect + Rect(2, 2, 0, 0), 5 * 2, 10, nvgRGBA(0, 0, 0, 128), nvgRGBA(0, 0, 0, 0));
	nvgBeginPath(context);
	nvgRect(context, rect + Rect(-10, -10, 20, 20));
	nvgRoundedRect(context, rect, 5);
	nvgPathWinding(context, NVG_HOLE);
	nvgFillPaint(context, shadowPaint);
	nvgFill(context);
	nvgClosePath(context);
}
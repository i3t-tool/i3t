#include "RenderOptions.h"

using namespace Vp;

RenderOptions::RenderOptions(bool multisample, int samples, bool framebufferAlpha, bool wboit)
    : multisample(multisample), samples(samples), framebufferAlpha(framebufferAlpha), wboit(wboit)
{
	// Empty
}
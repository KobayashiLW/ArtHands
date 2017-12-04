#include "RenderEngine.h"
#include "DeferredRender.h"


RenderEngine::RenderEngine()
{
	ptr_render_ = new DeferredRender();
}


RenderEngine::~RenderEngine()
{
}


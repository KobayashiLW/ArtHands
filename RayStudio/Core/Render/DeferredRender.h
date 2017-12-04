#pragma once
#include <D3D11.h>
#include "../Texture2D.h"
#include "../../Editor/Common/InfoManager.h"
#include "../../Editor/Common/DeviceManager.h"
#include "Render.h"

class DeferredRender : public Render
{
public:
	Texture2D *GBufferAbledo;
	Texture2D *GBufferNormal;

	InfoManager *InfoStream;
	DeviceManager *DeviceStream;
public:
	DeferredRender();
	~DeferredRender();

	void BuildGBuffer();
};


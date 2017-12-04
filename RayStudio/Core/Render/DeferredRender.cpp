#include "DeferredRender.h"



DeferredRender::DeferredRender()
{

}


DeferredRender::~DeferredRender()
{
}

void DeferredRender::BuildGBuffer()
{
	DXGI_SAMPLE_DESC sample_desc_;

	GBufferAbledo = new  Texture2D(DeviceStream->ptr_id3d_device_, InfoStream->WndWidth, InfoStream->WndHeight, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, sample_desc_);
	GBufferNormal = new  Texture2D(DeviceStream->ptr_id3d_device_, InfoStream->WndWidth, InfoStream->WndHeight, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, sample_desc_);



}
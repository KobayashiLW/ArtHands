#pragma once
#include <D3D11.h>
#include <D3DX11async.h>

class InfoManager;
class InputManager;

class DeviceManager
{
public:

	ID3D11Device*            ptr_id3d_device_;
	ID3D11DeviceContext*     ptr_device_context_;
	IDXGISwapChain*          ptr_swap_chain_;
	ID3D11RenderTargetView*  ptr_render_target_view_;
	ID3D11DepthStencilView*  ptr_depth_stencil_view_;

	// Data
	ID3D11Device*            gptr_d3d_device_ = NULL;
	ID3D11DeviceContext*     gptr_d3d_device_context_ = NULL;
	IDXGISwapChain*          gptr_swap_chain_ = NULL;
	ID3D11RenderTargetView*  gptr_main_render_target_view_ = NULL;



    // other device 
	InfoManager *InfManager;

public:
	DeviceManager();
	DeviceManager(InfoManager *DevManager);
	~DeviceManager();

	void Init(InfoManager &stream);
	void Update();


	HRESULT Create(char &DeviceName);
	HRESULT CreateD3d11Device();
	HRESULT CreateDeviceD3D(HWND hWnd);

	void CreateRenderTarget(ID3D11RenderTargetView *&InRenderTarget);

	void CleanupRenderTarget(ID3D11RenderTargetView *&InRenderTarget);
	void CleanupDeviceD3D();
};


#include "DeviceManager.h"
#include "InfoManager.h"


DeviceManager::DeviceManager()
{
}

DeviceManager::DeviceManager(InfoManager *InfoManager)
{
	InfManager = InfoManager;
}


DeviceManager::~DeviceManager()
{
}

void DeviceManager::Init(InfoManager &stream)
{
	InfManager = &stream;
}
void DeviceManager::Update()
{
	
}


HRESULT DeviceManager::Create(char &DeviceName)
{
	return S_OK;
}

HRESULT DeviceManager::CreateD3d11Device()
{
	//�����豸��������
	D3D_FEATURE_LEVEL features[3] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_3 };
	D3D_FEATURE_LEVEL myFeatureLevel;
	HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, 0, features, 3, D3D11_SDK_VERSION, &ptr_id3d_device_, &myFeatureLevel, &ptr_device_context_);
	if (FAILED(hr))
	{
	//	MessageBox(NULL, "����d3d11�豸ʧ�ܣ�", "����", MB_OK);
		return S_FALSE;
	}

	//���4x�����ȼ�
	UINT msaa4xQuality(0);
	ptr_id3d_device_->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaa4xQuality);

	//����������
	//������������
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferDesc.Width = InfManager->WndWidth;					//����
	swapChainDesc.BufferDesc.Height = InfManager->WndHeight;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;			//ˢ����
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					//�㶨��������������ָ������
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	//ͬ��
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;						//���ݸ�ʽ��һ��ΪRGBA��Ԫɫ��ʽ
	swapChainDesc.BufferCount = 1;														//�󻺳���������1���㹻
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;						//Usage���ܺ����
	swapChainDesc.Flags = 0;
	swapChainDesc.OutputWindow = InfManager->HWnd;							//�����ھ��
	swapChainDesc.SampleDesc.Count = 1;								//���ز���
	swapChainDesc.SampleDesc.Quality = msaa4xQuality - 1;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;			//������������������DISCARD
	swapChainDesc.Windowed = true;									//����ģʽ

																	//��ȡIDXGIFactory�Դ���������
	IDXGIDevice *dxgiDevice(NULL);
	ptr_id3d_device_->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
	IDXGIAdapter *dxgiAdapter(NULL);
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));
	IDXGIFactory *dxgiFactory(NULL);
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));
	hr = dxgiFactory->CreateSwapChain(ptr_id3d_device_, &swapChainDesc, &ptr_swap_chain_);
	if (FAILED(hr))
	{
		//MessageBox(NULL, "����d3d11������ʧ�ܣ�", "����", MB_OK);
		return S_FALSE;
	}
	dxgiFactory->Release();
	dxgiAdapter->Release();
	dxgiDevice->Release();

	//������ͼ
	//��Ҫ�Ȼ����Դ
	ID3D11Texture2D *backBuffer(NULL);
	ptr_swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	hr = ptr_id3d_device_->CreateRenderTargetView(backBuffer, NULL, &ptr_render_target_view_);
	if (FAILED(hr))
	{
		//MessageBox(NULL, "����RenderTargetViewʧ�ܣ�", "����", MB_OK);
		return S_FALSE;
	}
	backBuffer->Release();

	//������Ȼ�������ͼ
	//Ҫ�ȴ�����Ӧ������
	//����������Texture2D��Ҫ�ȸ�������
	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = InfManager->WndWidth;
	desc.Height = InfManager->WndHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.MiscFlags = 0;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = msaa4xQuality - 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	ID3D11Texture2D *depthStencilBuffer(NULL);
	hr = ptr_id3d_device_->CreateTexture2D(&desc, NULL, &depthStencilBuffer);
	if (FAILED(hr))
	{
	//	MessageBox(NULL, "����d3d11��Ȼ�����ʧ�ܣ�", "����", MB_OK);
		return S_FALSE;
	}

	// Initialize the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = ptr_id3d_device_->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &ptr_depth_stencil_view_);
	if (FAILED(hr))
	{
	//	MessageBox(NULL, "����DepthStencilViewʧ�ܣ�", "����", MB_OK);
		return S_FALSE;
	}

	//������ͼҪ�󶨵�������Ӧ�׶�
	ptr_device_context_->OMSetRenderTargets(1, &ptr_render_target_view_, ptr_depth_stencil_view_);
	depthStencilBuffer->Release();

	//��������ӿ�
	D3D11_VIEWPORT viewPort = { 0 };
	viewPort.Width = static_cast<FLOAT>(InfManager->WndWidth);
	viewPort.Height = static_cast<FLOAT>(InfManager->WndHeight);
	viewPort.MinDepth = 0.f;
	viewPort.MaxDepth = 1.f;
	viewPort.TopLeftX = 0.f;
	viewPort.TopLeftY = 0.f;
	ptr_device_context_->RSSetViewports(0, &viewPort);

	return S_OK;
}

void DeviceManager::CreateRenderTarget(ID3D11RenderTargetView *&InRenderTarget)
{
	DXGI_SWAP_CHAIN_DESC sd;
	gptr_swap_chain_->GetDesc(&sd);

	// Create the render target
	ID3D11Texture2D* pBackBuffer;
	D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;
	ZeroMemory(&render_target_view_desc, sizeof(render_target_view_desc));
	render_target_view_desc.Format = sd.BufferDesc.Format;
	render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	gptr_swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	gptr_d3d_device_->CreateRenderTargetView(pBackBuffer, &render_target_view_desc, &InRenderTarget);
	gptr_d3d_device_context_->OMSetRenderTargets(1, &InRenderTarget, NULL);
	pBackBuffer->Release();
}

void DeviceManager::CleanupRenderTarget(ID3D11RenderTargetView *&InRenderTarget)
{
	if (InRenderTarget) { InRenderTarget->Release(); InRenderTarget = NULL; }
}

HRESULT DeviceManager::CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	{
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[1] = { D3D_FEATURE_LEVEL_11_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 1, D3D11_SDK_VERSION, &sd, &gptr_swap_chain_, &gptr_d3d_device_, &featureLevel, &gptr_d3d_device_context_) != S_OK)
		return E_FAIL;

	CreateRenderTarget(gptr_main_render_target_view_);

	return S_OK;
}

void DeviceManager::CleanupDeviceD3D()
{
	CleanupRenderTarget(gptr_main_render_target_view_);
	if (gptr_swap_chain_) { gptr_swap_chain_->Release(); gptr_swap_chain_ = NULL; }
	if (gptr_d3d_device_context_) { gptr_d3d_device_context_->Release(); gptr_d3d_device_context_ = NULL; }
	if (gptr_d3d_device_) { gptr_d3d_device_->Release(); gptr_d3d_device_ = NULL; }
}




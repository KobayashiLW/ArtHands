#pragma once
#include <D3D11.h>
#include "Math\MathUtility.h"
#include<vector>

using std::vector;

struct Texture2D
{
public:
	// Construct a Texture2D / Texture2DArray / Texture2DMS / Texture2DMSArray
	Texture2D(ID3D11Device* d3dDevice, int32 width, int32 height, DXGI_FORMAT format, uint32 bindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, int mipLevels = 1);
	Texture2D(ID3D11Device* d3dDevice, int32 width, int32 height, DXGI_FORMAT format, uint32 bindFlags, int mipLevels, int arraySize);
	Texture2D(ID3D11Device* d3dDevice, int32 width, int32 height, DXGI_FORMAT format, uint32 bindFlags, const DXGI_SAMPLE_DESC& sampleDesc);
	Texture2D(ID3D11Device* d3dDevice, int32 width, int32 height, DXGI_FORMAT format, uint32 bindFlags, int arraySize, const DXGI_SAMPLE_DESC& sampleDesc);

	~Texture2D();

	ID3D11Texture2D*			GetTexture() { return mTexture; }
	ID3D11RenderTargetView*		GetRenderTarget(std::size_t arrayIndex = 0) { return mRenderTargetElements[arrayIndex]; }
	ID3D11UnorderedAccessView*	GetUnorderedAccess(std::size_t arrayIndex = 0) { return mUnorderedAccessElements[arrayIndex]; }	// Treat these like render targets for now... i.e. only access a slice
	ID3D11ShaderResourceView*	GetShaderResource() { return mShaderResource; }							// Get a full view of the resource
	ID3D11ShaderResourceView*	GetShaderResource(std::size_t arrayIndex) { return mShaderResourceElements[arrayIndex]; }		// Get a view to the top mip of a single array element

private:
	void InternalConstruct(ID3D11Device* d3dDevice,
		int32 width, int32 height, DXGI_FORMAT format,
		uint32 bindFlags, int32 mipLevels, int32 arraySize,
		int32 sampleCount, int32 sampleQuality,
		D3D11_RTV_DIMENSION rtvDimension,
		D3D11_UAV_DIMENSION uavDimension,
		D3D11_SRV_DIMENSION srvDimension);

	// Not implemented
	Texture2D(const Texture2D&);
	Texture2D& operator=(const Texture2D&);

	ID3D11Texture2D*			mTexture;
	ID3D11ShaderResourceView*	mShaderResource;

	// One per array element
	std::vector<ID3D11RenderTargetView*>		mRenderTargetElements;
	std::vector<ID3D11UnorderedAccessView*>		mUnorderedAccessElements;
	std::vector<ID3D11ShaderResourceView*>		mShaderResourceElements;
};

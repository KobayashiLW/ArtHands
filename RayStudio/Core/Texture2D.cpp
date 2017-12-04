#include"Texture2D.h"

//////////////////////////////////////////////////////////////////////////
Texture2D::Texture2D(ID3D11Device* d3dDevice, int32 width, int32 height, DXGI_FORMAT format, uint32 bindFlags, int32 mipLevels)
{
	InternalConstruct(d3dDevice, width, height, format, bindFlags, mipLevels, 1, 1, 0, D3D11_RTV_DIMENSION_TEXTURE2D, D3D11_UAV_DIMENSION_TEXTURE2D, D3D11_SRV_DIMENSION_TEXTURE2D);
}
//------------------------------------------------------------------------
Texture2D::Texture2D(ID3D11Device* d3dDevice, int32 width, int32 height, DXGI_FORMAT format, uint32 bindFlags, int32 mipLevels, int32 arraySize)
{
	InternalConstruct(d3dDevice, width, height, format, bindFlags, mipLevels, arraySize, 1, 0, D3D11_RTV_DIMENSION_TEXTURE2DARRAY, D3D11_UAV_DIMENSION_TEXTURE2DARRAY, D3D11_SRV_DIMENSION_TEXTURE2DARRAY);
}
//------------------------------------------------------------------------
Texture2D::Texture2D(ID3D11Device* d3dDevice, int32 width, int32 height, DXGI_FORMAT format, uint32 bindFlags, const DXGI_SAMPLE_DESC& sampleDesc)
{	// UAV's can't point to multisampled resources
	InternalConstruct(d3dDevice, width, height, format, bindFlags, 1, 1, sampleDesc.Count, sampleDesc.Quality, D3D11_RTV_DIMENSION_TEXTURE2DMS, D3D11_UAV_DIMENSION_UNKNOWN, D3D11_SRV_DIMENSION_TEXTURE2DMS);
}
//------------------------------------------------------------------------
Texture2D::Texture2D(ID3D11Device* d3dDevice, int32 width, int32 height, DXGI_FORMAT format, uint32 bindFlags, int32 arraySize, const DXGI_SAMPLE_DESC& sampleDesc)
{	// UAV's can't point to multisampled resources
	InternalConstruct(d3dDevice, width, height, format, bindFlags, 1, arraySize, sampleDesc.Count, sampleDesc.Quality, D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY, D3D11_UAV_DIMENSION_UNKNOWN, D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY);
}

//////////////////////////////////////////////////////////////////////////
void Texture2D::InternalConstruct(ID3D11Device* d3dDevice,
	int32 width, int32 height, DXGI_FORMAT format,
	uint32 bindFlags, int32 mipLevels, int32 arraySize,
	int32 sampleCount, int32 sampleQuality,
	D3D11_RTV_DIMENSION rtvDimension,
	D3D11_UAV_DIMENSION uavDimension,
	D3D11_SRV_DIMENSION srvDimension)
{
	mShaderResource = nullptr;

	CD3D11_TEXTURE2D_DESC desc(format,
		width, height, arraySize, mipLevels,
		bindFlags,
		D3D11_USAGE_DEFAULT, 0,
		sampleCount, sampleQuality,
		// If they request mipmap levels, it's nice to be able to autogenerate them.
		(mipLevels != 1 ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0));

	d3dDevice->CreateTexture2D(&desc, 0, &mTexture);

	// Update with actual mip levels, etc.
	mTexture->GetDesc(&desc);

	if (bindFlags & D3D11_BIND_RENDER_TARGET)
	{
		for (int i = 0; i < arraySize; ++i)
		{
			CD3D11_RENDER_TARGET_VIEW_DESC rtvDesc(rtvDimension, format, 0, i, 1);

			ID3D11RenderTargetView* renderTargetView;
			d3dDevice->CreateRenderTargetView(mTexture, &rtvDesc, &renderTargetView);
			mRenderTargetElements.push_back(renderTargetView);
		}
	}

	if (bindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		// UAV's can't point to multisampled resources!
		//RJE_ASSERT(uavDimension != D3D11_UAV_DIMENSION_UNKNOWN);
		

		for (int i = 0; i < arraySize; ++i)
		{
			CD3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc(uavDimension, format, 0, i, 1);

			ID3D11UnorderedAccessView* unorderedAccessView;
			d3dDevice->CreateUnorderedAccessView(mTexture, &uavDesc, &unorderedAccessView);
			mUnorderedAccessElements.push_back(unorderedAccessView);
		}
	}

	if (bindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		// Whole resource
		CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(srvDimension, format, 0, desc.MipLevels, 0, desc.ArraySize);
		d3dDevice->CreateShaderResourceView(mTexture, &srvDesc, &mShaderResource);

		// Single elements
		for (int i = 0; i < arraySize; ++i)
		{
			CD3D11_SHADER_RESOURCE_VIEW_DESC srvElementDesc(srvDimension, format, 0, 1, i, 1);

			ID3D11ShaderResourceView* shaderResourceView;
			d3dDevice->CreateShaderResourceView(mTexture, &srvElementDesc, &shaderResourceView);
			mShaderResourceElements.push_back(shaderResourceView);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
Texture2D::~Texture2D()
{
	for (std::size_t i = 0; i < mRenderTargetElements.size(); ++i)			mRenderTargetElements[i]->Release();
	for (std::size_t i = 0; i < mUnorderedAccessElements.size(); ++i)		mUnorderedAccessElements[i]->Release();
	for (std::size_t i = 0; i < mShaderResourceElements.size(); ++i)		mShaderResourceElements[i]->Release();

	if (mShaderResource)
		mShaderResource->Release();

	mTexture->Release();
}
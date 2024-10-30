#include "SwapChain.h"
#include "RenderSystem.h"

#include <iostream>
#include <exception>

SwapChain::SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system) : m_system(system)
{
	ID3D11Device* device = m_system->m_d3d_device;

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = true;

	HRESULT hr = m_system->m_dxgi_factory->CreateSwapChain(device, &desc, &m_swap_chain);

	if (FAILED(hr))
	{
		throw std::exception("SwapChain not created successfully");
	}

	ID3D11Texture2D* buffer = NULL;
	hr = m_swap_chain->GetBuffer(0, _uuidof(ID3D11Texture2D), (void**)&buffer);
	if (FAILED(hr))
	{
		throw std::exception("SwapChain not created successfully: BackBuffer failed");
	}

	hr = device->CreateRenderTargetView(buffer, NULL, &m_backbufferRTV);
	buffer->Release();
	if (FAILED(hr))
	{
		throw std::exception("SwapChain not created successfully: RenderTargetView failed");
	}

	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.MipLevels = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.MiscFlags = 0;
	texDesc.ArraySize = 1;
	texDesc.CPUAccessFlags = 0;

	hr = device->CreateTexture2D(&texDesc, NULL, &buffer);
	if (FAILED(hr))
	{
		std::cout << "whoops\n";
		throw std::exception("SwapChain not created successfully: DepthRenderView failed");
	}

	hr = device->CreateDepthStencilView(buffer, NULL, &this->depthView);
	if (FAILED(hr))
	{
		throw std::exception("SwapChain not created successfully: DepthStencilView failed");
	}
}

bool SwapChain::present(bool vsync)
{
	m_swap_chain->Present(vsync,NULL);
	return true;
}

SwapChain::~SwapChain()
{
	if(m_swap_chain)m_swap_chain->Release();
}

ID3D11RenderTargetView* SwapChain::getRenderTargetView()
{
	return this->m_backbufferRTV;
}

ID3D11DepthStencilView* SwapChain::getDepthStencilView()
{
	return this->depthView;
}

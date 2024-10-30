#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system);
	bool present(bool vsync);
	~SwapChain();

	ID3D11RenderTargetView* getRenderTargetView();
	ID3D11DepthStencilView* getDepthStencilView();
private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_backbufferRTV;
	ID3D11DepthStencilView* depthView;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};


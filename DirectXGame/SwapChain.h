#pragma once
#include <d3d11.h>
class DeviceContext;
class SwapChain
{
public:
	SwapChain();
	bool init(HWND hwnd, UINT width, UINT height);

	bool present(bool vsync);

	bool release();
	~SwapChain();

	ID3D11RenderTargetView* getRenderTargetView();
	ID3D11DepthStencilView* getDepthStencilView();
private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_backbufferRTV;
	ID3D11DepthStencilView* depthView;

private:
	friend class DeviceContext;
};


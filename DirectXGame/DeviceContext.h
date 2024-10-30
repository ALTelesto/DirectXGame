#pragma once
#include <d3d11.h>

#include "SamplerState.h"
#include "Prerequisites.h"


class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* system);
	void clearRenderTargetColor(SwapChainPtr swap_chain, float red, float green, float blue, float alpha);
	void clearRenderTargetColor(ID3D11RenderTargetView* render_target_view, ID3D11DepthStencilView* depth_stencil_view, float red, float green, float blue, float alpha);
	void clearRenderTargetView(SwapChainPtr swap_chain, ID3D11RenderTargetView* renderView, float red, float green,
	                           float blue, float alpha);
	void setVertexBuffer(VertexBufferPtr vertex_buffer);
	void setIndexBuffer(IndexBufferPtr index_buffer);

	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	void setViewportSize(UINT width, UINT height);

	void setVertexShader(VertexShaderPtr vertex_shader);
	void setPixelShader(PixelShaderPtr pixel_shader);

	void setSamplerState(SamplerStatePtr sampler_state);

	void setConstantBuffer(VertexShaderPtr vertex_shader, ConstantBufferPtr buffer);
	void setConstantBuffer(PixelShaderPtr pixel_shader, ConstantBufferPtr buffer);

	//render to image
	void setRenderTargets(ID3D11RenderTargetView* render_target_view, ID3D11DepthStencilView* depth_stencil_view);
	void setBackBufferRenderTargets(SwapChainPtr swap_chain);
	void setShaderResources(UINT start_slot, UINT num_views, ID3D11ShaderResourceView** shader_resource_views);
	void unbindShaderResources();

	void setDepthStencilState(ID3D11DepthStencilState* depth_stencil_state);

	void Dispatch(UINT thread_group_count_x, UINT thread_group_count_y, UINT thread_group_count_z);

	ID3D11DeviceContext* getContext();

	~DeviceContext();
private:
	ID3D11DeviceContext* m_device_context;
	RenderSystem* m_system = nullptr;
private:
	friend class ConstantBuffer;
};


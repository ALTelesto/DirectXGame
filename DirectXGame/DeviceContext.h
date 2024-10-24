#pragma once
#include <d3d11.h>

#include "SamplerState.h"

class SwapChain;
class VertexBuffer;
class ConstantBuffer;
class IndexBuffer;
class VertexShader;
class PixelShader;


class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context);
	void clearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha);
	void clearRenderTargetColor(ID3D11RenderTargetView* render_target_view, ID3D11DepthStencilView* depth_stencil_view, float red, float green, float blue, float alpha);
	void clearRenderTargetView(SwapChain* swap_chain, ID3D11RenderTargetView* renderView, float red, float green,
	                           float blue, float alpha);
	void setVertexBuffer(VertexBuffer* vertex_buffer);
	void setIndexBuffer(IndexBuffer* index_buffer);

	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	void setViewportSize(UINT width, UINT height);

	void setVertexShader(VertexShader* vertex_shader);
	void setPixelShader(PixelShader* pixel_shader);

	void setSamplerState(SamplerState* sampler_state);

	void setConstantBuffer(VertexShader* vertex_shader, ConstantBuffer* buffer);
	void setConstantBuffer(PixelShader* pixel_shader, ConstantBuffer* buffer);

	//render to image
	void setRenderTargets(ID3D11RenderTargetView* render_target_view, ID3D11DepthStencilView* depth_stencil_view);
	void setBackBufferRenderTargets(SwapChain* swap_chain);
	void setShaderResources(UINT start_slot, UINT num_views, ID3D11ShaderResourceView** shader_resource_views);
	void unbindShaderResources();

	void setDepthStencilState(ID3D11DepthStencilState* depth_stencil_state);

	void Dispatch(UINT thread_group_count_x, UINT thread_group_count_y, UINT thread_group_count_z);

	bool release();
	~DeviceContext();
	

private:
	ID3D11DeviceContext* m_device_context;
private:
	friend class ConstantBuffer;
};


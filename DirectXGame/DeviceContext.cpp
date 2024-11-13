#include "DeviceContext.h"

#include "PixelShader.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "SamplerState.h"
#include "Material.h"
#include <exception>

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* system):m_device_context(device_context), m_system(system)
{
}

void DeviceContext::clearRenderTargetColor(SwapChainPtr swap_chain,float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red,green,blue,alpha};
	ID3D11RenderTargetView* renderView = swap_chain->getRenderTargetView();
	ID3D11DepthStencilView* depthView = swap_chain->getDepthStencilView();
	m_device_context->ClearRenderTargetView(renderView, clear_color);
	m_device_context->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	m_device_context->OMSetRenderTargets(1, &renderView, depthView);
}

void DeviceContext::clearRenderTargetColor(ID3D11RenderTargetView* render_target_view, ID3D11DepthStencilView* depth_stencil_view, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red,green,blue,alpha };
	m_device_context->ClearRenderTargetView(render_target_view, clear_color);
	m_device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	m_device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);
}

void DeviceContext::clearRenderTargetView(SwapChainPtr swap_chain, ID3D11RenderTargetView* renderView, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red,green,blue,alpha };
	ID3D11DepthStencilView* depthView = swap_chain->getDepthStencilView();
	m_device_context->OMSetRenderTargets(1, &renderView, depthView);
	//m_device_context->ClearRenderTargetView(swap_chain->getRenderTargetView(), clear_color);
	m_device_context->ClearRenderTargetView(renderView, clear_color);
	m_device_context->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	
}

void DeviceContext::setVertexBuffer(VertexBufferPtr vertex_buffer)
{
	UINT strider = vertex_buffer->m_size_vertex;
	UINT offset = 0;
	m_device_context->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer,&strider, &offset);
	m_device_context->IASetInputLayout(vertex_buffer->m_layout);
}

void DeviceContext::setIndexBuffer(IndexBufferPtr index_buffer)
{
	m_device_context->IASetIndexBuffer(index_buffer->m_buffer,DXGI_FORMAT_R32_UINT,0);
}

void DeviceContext::drawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->DrawIndexed(index_count, start_index_location, start_vertex_index);
}

void DeviceContext::drawTriangleStrip(UINT vertex_count, UINT start_vertex_index)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_device_context->Draw(vertex_count, start_vertex_index);
}



void DeviceContext::setViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT vp = {};
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_device_context->RSSetViewports(1, &vp);
}

void DeviceContext::setVertexShader(VertexShaderPtr vertex_shader)
{
	m_device_context->VSSetShader(vertex_shader->m_vs, nullptr, 0);
}

void DeviceContext::setPixelShader(PixelShaderPtr pixel_shader)
{
	m_device_context->PSSetShader(pixel_shader->m_ps, nullptr, 0);
}

void DeviceContext::setSamplerState(SamplerStatePtr sampler_state)
{
	m_device_context->PSSetSamplers(0, 1, &sampler_state->m_sampler_state);
}

void DeviceContext::setConstantBuffer(VertexShaderPtr vertex_shader, ConstantBufferPtr buffer)
{
	m_device_context->VSSetConstantBuffers(0,1,&buffer->m_buffer);
}

void DeviceContext::setConstantBuffer(PixelShaderPtr pixel_shader, ConstantBufferPtr buffer)
{
	m_device_context->PSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

void DeviceContext::setRenderTargets(ID3D11RenderTargetView* render_target_view, ID3D11DepthStencilView* depth_stencil_view)
{
	m_device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);
}
void DeviceContext::setBackBufferRenderTargets(SwapChainPtr swap_chain)
{
	ID3D11RenderTargetView* renderView = swap_chain->getRenderTargetView();
	m_device_context->OMSetRenderTargets(1, &renderView, swap_chain->getDepthStencilView());
}

void DeviceContext::setShaderResources(UINT start_slot, UINT num_views, ID3D11ShaderResourceView** shader_resource_views)
{
	m_device_context->PSSetShaderResources(start_slot, num_views, shader_resource_views);
}
void DeviceContext::unbindShaderResources()
{
	constexpr ID3D11ShaderResourceView* nullSRV = nullptr;
	m_device_context->PSSetShaderResources(0, 1, &nullSRV);
}

void DeviceContext::setTextures(MaterialPtr material)
{
	m_device_context->VSSetShaderResources(0, 1, material->albedoTexture.GetAddressOf());
	m_device_context->VSSetShaderResources(1, 1, material->normalTexture.GetAddressOf());
	m_device_context->VSSetShaderResources(2, 1, material->metallicTexture.GetAddressOf());
	m_device_context->VSSetShaderResources(3, 1, material->smoothnessTexture.GetAddressOf());
	m_device_context->VSSetSamplers(0, 1, &material->samplerState->m_sampler_state);

	m_device_context->PSSetShaderResources(0, 1, material->albedoTexture.GetAddressOf());
	m_device_context->PSSetShaderResources(1, 1, material->normalTexture.GetAddressOf());
	m_device_context->PSSetShaderResources(2, 1, material->metallicTexture.GetAddressOf());
	m_device_context->PSSetShaderResources(3, 1, material->smoothnessTexture.GetAddressOf());
	m_device_context->PSSetSamplers(0, 1, &material->samplerState->m_sampler_state);
}

void DeviceContext::setDepthStencilState(ID3D11DepthStencilState* depth_stencil_state)
{
	m_device_context->OMSetDepthStencilState(depth_stencil_state, 0);
}

void DeviceContext::Dispatch(UINT thread_group_count_x, UINT thread_group_count_y, UINT thread_group_count_z)
{
	m_device_context->Dispatch(thread_group_count_x, thread_group_count_y, thread_group_count_z);
}

ID3D11DeviceContext* DeviceContext::getContext()
{
	return this->m_device_context;
}

DeviceContext::~DeviceContext()
{
	if(m_device_context)m_device_context->Release();
}

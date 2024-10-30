#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Prerequisites.h"

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem();
public:
	SwapChainPtr createSwapChain(HWND hwnd, UINT width, UINT height);
	DeviceContextPtr getImmediateDeviceContext();
	VertexBufferPtr createVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader, D3D11_BUFFER_DESC
		buff_desc);
	ConstantBufferPtr createConstantBuffer(void* buffer, UINT size_buffer);
	IndexBufferPtr createIndexBuffer(void* list_indices, UINT size_list);
	SamplerStatePtr createSamplerState();
	VertexShaderPtr createVertexShader(const void* shader_byte_code, size_t byte_code_size);
	PixelShaderPtr createPixelShader(const void* shader_byte_code, size_t byte_code_size);
	bool createRenderTexture(ID3D11ShaderResourceView** srv, ID3D11RenderTargetView** rtv);
	bool createDepthStencilView(ID3D11DepthStencilView** dsv);
public:
	bool compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	bool compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	void releaseCompiledShader();
	void setToRenderTargetView(ID3D11RenderTargetView* render_target_view, ID3D11DepthStencilView* depth_stencil_view);
	void EnableDepthTest();
	void DisableDepthTest();

public:

private:
	DeviceContextPtr m_imm_device_context;
private:
	ID3D11Device* m_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;
private:
	IDXGIDevice* m_dxgi_device;
	IDXGIAdapter* m_dxgi_adapter;
	IDXGIFactory* m_dxgi_factory;
	ID3D11DeviceContext* m_imm_context;
	ID3D11DepthStencilState* m_depthStencilStateEnabled;
	ID3D11DepthStencilState* m_depthStencilStateDisabled;
private:
	ID3DBlob* m_blob = nullptr;
	ID3DBlob* m_vsblob = nullptr;
	ID3DBlob* m_psblob = nullptr;
	ID3D11VertexShader* m_vs = nullptr;
	ID3D11PixelShader* m_ps = nullptr;
private:
	friend class SwapChain;
	friend class VertexBuffer;
	friend class VertexShader;
	friend class PixelShader;
	friend class ConstantBuffer;
	friend class IndexBuffer;
	friend class SamplerState;
};


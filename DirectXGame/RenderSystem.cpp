#include "RenderSystem.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include <d3dcompiler.h>

#include "EngineTime.h"
#include "SamplerState.h"

#include "Settings.h"

#include <exception>

#include "AppWindow.h"

RenderSystem::RenderSystem()
{
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT res = 0;
	for (UINT driver_type_index = 0; driver_type_index < num_driver_types; )
	{
		res = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels,
			num_feature_levels, D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &m_imm_context);

		if (SUCCEEDED(res))
		{
			break;
		}
		++driver_type_index;
	}

	if (FAILED(res))
	{
		throw std::exception("RenderSystem not created successfully");
	}

	m_imm_device_context = std::make_shared<DeviceContext>(m_imm_context, this);

	m_d3d_device->QueryInterface(_uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(_uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(_uuidof(IDXGIFactory), (void**)&m_dxgi_factory);

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = FALSE;

	HRESULT hr = m_d3d_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilStateEnabled);
	if (FAILED(hr))
	{
		throw std::exception("RenderSystem not created successfully: DepthStencilState failure");
	}

	//m_imm_context->OMSetDepthStencilState(m_depthStencilStateEnabled, 0);

	depthStencilDesc.DepthEnable = FALSE;
	hr = m_d3d_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilStateDisabled);
	if (FAILED(hr))
	{
		throw std::exception("RenderSystem not created successfully: DepthStencilState failure");
	}
}

RenderSystem::~RenderSystem()
{
	if (m_dxgi_device)m_dxgi_device->Release();
	if (m_dxgi_adapter)m_dxgi_adapter->Release();
	if (m_dxgi_factory)m_dxgi_factory->Release();
	if (m_vsblob)m_vsblob->Release();
	if (m_psblob)m_psblob->Release();
	m_d3d_device->Release();
}

SwapChainPtr RenderSystem::createSwapChain(HWND hwnd, UINT width, UINT height)
{
	SwapChainPtr sc = nullptr;
	try
	{
		sc = std::make_shared<SwapChain>(hwnd, width, height, this);
	}
	catch(...){}
	return sc;
}

DeviceContextPtr RenderSystem::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}

VertexBufferPtr RenderSystem::createVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list,
	void* shader_byte_code, UINT size_byte_shader)
{
	VertexBufferPtr vb = nullptr;
	try
	{
		vb = std::make_shared<VertexBuffer>(list_vertices, size_vertex, size_list, shader_byte_code, size_byte_shader, this);
	}
	catch (...) {}
	return vb;
}

VertexBufferPtr RenderSystem::createVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader, D3D11_BUFFER_DESC
                                                 buff_desc)
{
	VertexBufferPtr vb = nullptr;
	try
	{
		vb = std::make_shared<VertexBuffer>(list_vertices, size_vertex, size_list, shader_byte_code, size_byte_shader, buff_desc, this);
	}
	catch (...) {}
	return vb;
}

ConstantBufferPtr RenderSystem::createConstantBuffer(void* buffer, UINT size_buffer)
{
	ConstantBufferPtr cb = nullptr;
	try
	{
		cb = std::make_shared<ConstantBuffer>(buffer, size_buffer, this);
	}
	catch (...) {}
	return cb;
}

IndexBufferPtr RenderSystem::createIndexBuffer(void* list_indices, UINT size_list)
{
	IndexBufferPtr ib = nullptr;
	try
	{
		ib = std::make_shared<IndexBuffer>(list_indices, size_list, this);
	}
	catch (...) {}
	return ib;
}

SamplerStatePtr RenderSystem::createSamplerState()
{
	SamplerStatePtr ss = nullptr;
	try
	{
		ss = std::make_shared<SamplerState>(this);
	}
	catch(...) {}
	return ss;
}

VertexShaderPtr RenderSystem::createVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
	VertexShaderPtr vs = nullptr;
	try
	{
		vs = std::make_shared<VertexShader>(shader_byte_code, byte_code_size, this);
	}catch(...) {}
	return vs;
}

PixelShaderPtr RenderSystem::createPixelShader(const void* shader_byte_code, size_t byte_code_size)
{
	PixelShaderPtr ps = nullptr;
	try
	{
		ps = std::make_shared<PixelShader>(shader_byte_code, byte_code_size, this);
	}catch(...) {}
	return ps;
}

bool RenderSystem::createRenderTexture(ID3D11ShaderResourceView** srv, ID3D11RenderTargetView** rtv)
{
	//std::cout << "createRenderTexture srv edition\n";
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = AppWindow::getInstance()->getWidth();
	textureDesc.Height = AppWindow::getInstance()->getHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	ID3D11Texture2D* texture = nullptr;
	HRESULT hr = m_d3d_device->CreateTexture2D(&textureDesc, NULL, &texture);
	if (FAILED(hr)) {
		std::cout << "Failed to create texture\n";
		return false;
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	hr = m_d3d_device->CreateRenderTargetView(texture, &renderTargetViewDesc, rtv);
	if (FAILED(hr)) {
		std::cout << "Failed to create render target view\n";
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	hr = m_d3d_device->CreateShaderResourceView(texture, &srvDesc, srv);
	if (FAILED(hr)) {
		std::cout << "Failed to create shader resource view\n";
		return false;
	}

	texture->Release();
	return true;
}

bool RenderSystem::createDepthStencilView(ID3D11DepthStencilView** dsv)
{
	HRESULT hr;
	ID3D11Texture2D* texture = nullptr;

	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = AppWindow::getInstance()->getWidth();
	texDesc.Height = AppWindow::getInstance()->getHeight();
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.MipLevels = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.MiscFlags = 0;
	texDesc.ArraySize = 1;
	texDesc.CPUAccessFlags = 0;

	hr = m_d3d_device->CreateTexture2D(&texDesc, NULL, &texture);
	if (FAILED(hr))
	{
		std::cout << "dsv failure 1 \n";
		return false;
	}

	hr = m_d3d_device->CreateDepthStencilView(texture, NULL, dsv);
	if (FAILED(hr))
	{
		std::cout << "dsv failure 2 \n";
		return false;
	}
}

ID3D11Device* RenderSystem::getDirectXDevice()
{
	return this->m_d3d_device;
}

bool RenderSystem::compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(::D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &m_blob, &error_blob)))
	{
		if (error_blob)error_blob->Release();
		return false;
	}
	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();


	return true;
}

bool RenderSystem::compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code,
	size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(::D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &m_blob, &error_blob)))
	{
		if (error_blob)error_blob->Release();
		return false;
	}
	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();


	return true;
}

void RenderSystem::releaseCompiledShader()
{
	if (m_blob) m_blob->Release();
}


void RenderSystem::setToRenderTargetView(ID3D11RenderTargetView* render_target_view, ID3D11DepthStencilView* depth_stencil_view)
{
	m_imm_device_context->setRenderTargets(render_target_view, depth_stencil_view);
}

void RenderSystem::EnableDepthTest() {
	m_imm_context->OMSetDepthStencilState(m_depthStencilStateEnabled, 0);
}

void RenderSystem::DisableDepthTest() {
	m_imm_context->OMSetDepthStencilState(m_depthStencilStateDisabled, 0);
}


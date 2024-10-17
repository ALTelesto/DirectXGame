#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include <d3dcompiler.h>

#include "EngineTime.h"

#include "Settings.h"

GraphicsEngine* GraphicsEngine::sharedInstance = nullptr;

GraphicsEngine* GraphicsEngine::getInstance()
{
	return sharedInstance;
}

GraphicsEngine::GraphicsEngine()
{
}

GraphicsEngine::~GraphicsEngine()
{
}

void GraphicsEngine::initialize()
{
	sharedInstance = new GraphicsEngine();
	sharedInstance->init();
	EngineTime::initialize();
}

void GraphicsEngine::destroy()
{
	if(sharedInstance != NULL)
	{
		sharedInstance->release();
		delete sharedInstance;
	}
}

bool GraphicsEngine::init()
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
	for(UINT driver_type_index = 0; driver_type_index< num_driver_types; )
	{
		res = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels,
			num_feature_levels, D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &m_imm_context);

		if(SUCCEEDED(res))
		{
			break;
		}
		++driver_type_index;
	}

	if (FAILED(res))
	{
		return false;
	}

	m_imm_device_context = new DeviceContext(m_imm_context);

	m_d3d_device->QueryInterface(_uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(_uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(_uuidof(IDXGIFactory),(void**)&m_dxgi_factory);

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = TRUE; 
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = FALSE;

	HRESULT hr = m_d3d_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(hr))
	{
		std::cout << "Failed to create Depth Stencil State\n";
		return false;
	}

	m_imm_context->OMSetDepthStencilState(m_depthStencilState, 0);


	//render to image stuff
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = WIDTH;
	textureDesc.Height = HEIGHT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	hr = m_d3d_device->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTexture);
	if (FAILED(hr))
	{
		std::cout << "Failed to create render target texture\n";
		return false;
	}

	hr = m_d3d_device->CreateRenderTargetView(m_renderTargetTexture, NULL, &m_renderTargetView);
	if (FAILED(hr))
	{
		std::cout << "Failed to create render target view\n";
		return false;
	}

	hr = m_d3d_device->CreateShaderResourceView(m_renderTargetTexture, NULL, &m_shaderResourceView);
	if (FAILED(hr))
	{
		std::cout << "Failed to create shader resource view\n";
		return false;
	}

	return true;
}

bool GraphicsEngine::release()
{
	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();

	if (m_vsblob)m_vsblob->Release();
	if (m_psblob)m_psblob->Release();

	m_imm_device_context->release();
	m_d3d_device->Release();
	return true;
}

SwapChain* GraphicsEngine::createSwapChain()
{
	return new SwapChain();
}

DeviceContext* GraphicsEngine::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}

VertexBuffer* GraphicsEngine::createVertexBuffer()
{
	return new VertexBuffer();
}

ConstantBuffer* GraphicsEngine::createConstantBuffer()
{
	return new ConstantBuffer();
}

IndexBuffer* GraphicsEngine::createIndexBuffer()
{
	return new IndexBuffer();
}

VertexShader* GraphicsEngine::createVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
	VertexShader* vs = new VertexShader();

	if(!vs->init(shader_byte_code,byte_code_size))
	{
		vs->release();
		return nullptr;
	}
	return vs;
}

PixelShader* GraphicsEngine::createPixelShader(const void* shader_byte_code, size_t byte_code_size)
{
	PixelShader* ps = new PixelShader();

	if (!ps->init(shader_byte_code, byte_code_size))
	{
		ps->release();
		return nullptr;
	}
	return ps;
}

ID3D11ShaderResourceView* GraphicsEngine::createShaderResourceView()
{
	if(m_renderTargetTexture == nullptr || m_renderTargetView == nullptr) return nullptr;

	ID3D11ShaderResourceView* shader_resource_view;

	HRESULT hr = m_d3d_device->CreateShaderResourceView(m_renderTargetTexture, NULL, &shader_resource_view);
	if (FAILED(hr))
	{
		std::cout << "Failed to create shader resource view\n";
		return nullptr;
	}
	return shader_resource_view;
}

bool GraphicsEngine::compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;
	if(!SUCCEEDED(::D3DCompileFromFile(file_name,nullptr,nullptr,entry_point_name,"vs_5_0",0,0, &m_blob, &error_blob)))
	{
		if(error_blob)error_blob->Release();
		return false;
	}
	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();


	return true;
}

bool GraphicsEngine::compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code,
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

void GraphicsEngine::releaseCompiledShader()
{
	if (m_blob) m_blob->Release();
}

void GraphicsEngine::setToRenderTexture(SwapChain* swap_chain)
{
	m_imm_device_context->setRenderTargets(m_renderTargetView, swap_chain->getDepthStencilView());
}



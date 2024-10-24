#include "SamplerState.h"

#include <iostream>

#include "DeviceContext.h"
#include "GraphicsEngine.h"

SamplerState::SamplerState()
{
}

bool SamplerState::load()
{
	if (m_sampler_state) m_sampler_state->Release();

	D3D11_SAMPLER_DESC sampler_desc = {};
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;  
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;     
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;    
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;     
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;   
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;



	if (FAILED(GraphicsEngine::getInstance()->m_d3d_device->CreateSamplerState(&sampler_desc, &m_sampler_state)))
	{
		std::cout << "samplerstate failure \n";
		return false;
	}

	return true;
}


bool SamplerState::release()
{
	if (m_sampler_state)m_sampler_state->Release();
	delete this;
	return true;
}

SamplerState::~SamplerState()
{
}

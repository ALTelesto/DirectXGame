#include "RasterizerState.h"

RasterizerState::RasterizerState()
{
}

RasterizerState::~RasterizerState()
{
    release();
}

bool RasterizerState::init(ID3D11Device* device, D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode)
{
    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = fillMode;               
    rasterDesc.CullMode = cullMode;               
    rasterDesc.DepthClipEnable = TRUE;            

    HRESULT hr = device->CreateRasterizerState(&rasterDesc, &m_rasterizerState);
    return SUCCEEDED(hr);
}

void RasterizerState::setRasterizerState(ID3D11DeviceContext* deviceContext)
{
    if (m_rasterizerState)
    {
        deviceContext->RSSetState(m_rasterizerState);
    }
}

void RasterizerState::release()
{
    if (m_rasterizerState)m_rasterizerState->Release();
}
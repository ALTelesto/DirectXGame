#pragma once
#include <d3d11.h>

class RasterizerState
{
public:
    RasterizerState();
    ~RasterizerState();

    bool init(ID3D11Device* device, D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode);
    void setRasterizerState(ID3D11DeviceContext* deviceContext);
    void release();

private:
    ID3D11RasterizerState* m_rasterizerState = nullptr;
};

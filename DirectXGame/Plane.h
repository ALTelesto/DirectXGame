#pragma once
#include "AGameObject.h"

class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;

class Plane : public AGameObject
{
public:
    Plane(string name, void* shaderByteCode, size_t sizeShader);
    ~Plane();

    void update(float deltaTime) override;
    void draw(int width, int height, VertexShaderPtr vertexShader, PixelShaderPtr pixelShader) override;
    void draw(const RECT clientWindow) override;

private:
    VertexBufferPtr vertexBuffer;
    IndexBufferPtr indexBuffer;
    ConstantBufferPtr constantBuffer;
    float deltaTime = 0.0f;
    float ticks = 0.0f;
    float speed = 10.0f;
};

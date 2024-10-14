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
    void draw(int width, int height, VertexShader* vertexShader, PixelShader* pixelShader) override;

private:
    VertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;
    ConstantBuffer* constantBuffer;
    float deltaTime = 0.0f;
    float ticks = 0.0f;
    float speed = 10.0f;
};

#pragma once
#include "AGameObject.h"

class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;

class Cube :	public AGameObject
{
public:
	Cube(string name, void* shaderByteCode, size_t sizeShader);
	~Cube();

	void update(float deltaTime) override;
	void draw(int width, int height, VertexShaderPtr vertexShader, PixelShaderPtr pixelShader) override;
	void setAnimSpeed(float speed);

private:
	VertexBufferPtr vertexBuffer;
	IndexBufferPtr indexBuffer;
	ConstantBufferPtr constantBuffer;
	float ticks = 0.0f;
	float deltaPos = 0.0f;
	float deltaTime = 0.0f;
	float speed = 10.0f;
};


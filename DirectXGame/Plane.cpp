#include "Plane.h"
#include "GraphicsEngine.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "IndexBuffer.h"

#include <iostream>

#include "AppWindow.h"

#include "LogUtils.h"

Plane::Plane(string name, void* shaderByteCode, size_t sizeShader) : AGameObject(name)
{
    GraphicsEngine* graphicsEngine = GraphicsEngine::getInstance();

    Vertex quadList[] = {
        { Vector3D(-0.5f, -0.5f, 0.0f), vec2(0,1)},  // bl
        { Vector3D(-0.5f, 0.5f, 0.0f), vec2(0,0) },   // tl
        { Vector3D(0.5f, 0.5f, 0.0f),  vec2(1,0) },    // tr
        { Vector3D(0.5f, -0.5f, 0.0f),   vec2(1,1) }    // br
    };

	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(quadList);
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

    this->vertexBuffer = graphicsEngine->getRenderSystem()->createVertexBuffer(quadList, sizeof(Vertex), ARRAYSIZE(quadList), shaderByteCode, sizeShader, buff_desc);

    unsigned int index_list[] = {
		0,1,2,
		2,3,0,
    };

    this->indexBuffer = graphicsEngine->getRenderSystem()->createIndexBuffer(index_list, ARRAYSIZE(index_list));

	CBData cbData = {};
	cbData.time = 0;
    this->constantBuffer = graphicsEngine->getRenderSystem()->createConstantBuffer(&cbData, sizeof(CBData));
}

Plane::~Plane()
{
}

void Plane::update(float deltaTime)
{
	this->deltaTime = deltaTime;

	this->ticks += deltaTime;

	/*float rotSpeed = this->ticks * this->speed;
	this->setRotation(rotSpeed, rotSpeed, rotSpeed);*/
}

//void Plane::draw(const RECT clientWindow)
//{
//	AGameObject::draw(clientWindow);
//}

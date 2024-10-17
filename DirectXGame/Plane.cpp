#include "Plane.h"
#include "GraphicsEngine.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "IndexBuffer.h"

#include <iostream>

#include "AppWindow.h"

Plane::Plane(string name, void* shaderByteCode, size_t sizeShader) : AGameObject(name)
{
    GraphicsEngine* graphicsEngine = GraphicsEngine::getInstance();

    Vertex quadList[] = {
        { Vector3D(-0.5f, -0.5f, 0.0f), vec2(-.5f,-.5f)},  // bl
        { Vector3D(-0.5f, 0.5f, 0.0f),  vec2(-.5f,.5f) },   // tl
        { Vector3D(0.5f, 0.5f, 0.0f),  vec2(.5f,.5f) },    // tr
        { Vector3D(0.5f, -0.5f, 0.0f),  vec2(.5f,-.5f) }    // br
    };

    this->vertexBuffer = graphicsEngine->createVertexBuffer();
    this->vertexBuffer->load(quadList, sizeof(Vertex), ARRAYSIZE(quadList), shaderByteCode, sizeShader);

    unsigned int index_list[] = {
		0,1,2,
		2,3,0,
    };

    this->indexBuffer = graphicsEngine->createIndexBuffer();
    this->indexBuffer->load(index_list, ARRAYSIZE(index_list));

    this->constantBuffer = graphicsEngine->createConstantBuffer();
    CBData cbData = {};
    cbData.time = 0;
    this->constantBuffer->load(&cbData, sizeof(CBData));
}

Plane::~Plane()
{
    this->vertexBuffer->release();
    this->indexBuffer->release();
    this->constantBuffer->release();
}

void Plane::update(float deltaTime)
{
	this->deltaTime = deltaTime;

	this->ticks += deltaTime;

	/*float rotSpeed = this->ticks * this->speed;
	this->setRotation(rotSpeed, rotSpeed, rotSpeed);*/
}

void Plane::draw(int width, int height, VertexShader* vertexShader, PixelShader* pixelShader)
{
	GraphicsEngine* graphicsEngine = GraphicsEngine::getInstance();
	DeviceContext* deviceContext = graphicsEngine->getImmediateDeviceContext();

	CBData cbData = {};
	cbData.time = 0;

	Vector3D rotation = this->getLocalRotation();
	Vector3D position = this->getLocalPosition();

	cbData.worldMatrix.setIdentity();
	cbData.worldMatrix.setScale(this->getLocalScale());

	Matrix4x4 rotMatrix;
	rotMatrix.setIdentity();

	rotMatrix.setRotationZ(rotation.m_z);
	cbData.worldMatrix *= rotMatrix;

	rotMatrix.setIdentity();
	rotMatrix.setRotationY(rotation.m_y);
	cbData.worldMatrix *= rotMatrix;

	rotMatrix.setIdentity();
	rotMatrix.setRotationX(rotation.m_x);
	cbData.worldMatrix *= rotMatrix;

	Matrix4x4 translationMatrix;
	translationMatrix.setIdentity();
	translationMatrix.setTranslation(position);
	cbData.worldMatrix *= translationMatrix;

	Matrix4x4 temp = AppWindow::getInstance()->getWorldCam();
	temp.inverse();
	cbData.viewMatrix = temp;
	cbData.projMatrix = AppWindow::getInstance()->getProjection();

	this->constantBuffer->update(deviceContext, &cbData);
	deviceContext->setConstantBuffer(vertexShader, this->constantBuffer);
	deviceContext->setConstantBuffer(pixelShader, this->constantBuffer);

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setVertexShader(vertexShader);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setPixelShader(pixelShader);

	deviceContext->setIndexBuffer(this->indexBuffer);
	deviceContext->setVertexBuffer(this->vertexBuffer);

	deviceContext->drawIndexedTriangleList(this->indexBuffer->getSizeIndexList(), 0, 0);

	//std::cout << "draw " << name << "\n";
	//std::cout << name <<" rot "<< rotation.m_x<<"  " << rotation.m_y << "  " << rotation.m_z << "  " << "\n";
}

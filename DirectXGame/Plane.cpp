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

void Plane::draw(int width, int height, VertexShaderPtr vertexShader, PixelShaderPtr pixelShader)
{
	GraphicsEngine* graphicsEngine = GraphicsEngine::getInstance();
	DeviceContextPtr deviceContext = graphicsEngine->getRenderSystem()->getImmediateDeviceContext();

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

	//Matrix4x4 temp = AppWindow::getInstance()->getWorldCam();
	Matrix4x4 temp = SceneCameraHandler::getInstance()->getSceneCameraViewMatrix();
	//temp.inverse();
	cbData.viewMatrix = temp;
	cbData.projMatrix = AppWindow::getInstance()->getProjection();

	this->constantBuffer->update(deviceContext, &cbData);
	deviceContext->setConstantBuffer(vertexShader, this->constantBuffer);
	deviceContext->setConstantBuffer(pixelShader, this->constantBuffer);

	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(vertexShader);
	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(pixelShader);

	deviceContext->setIndexBuffer(this->indexBuffer);
	deviceContext->setVertexBuffer(this->vertexBuffer);

	deviceContext->drawIndexedTriangleList(this->indexBuffer->getSizeIndexList(), 0, 0);

	//std::cout << "draw " << name << "\n";
	//std::cout << name <<" rot "<< rotation.m_x<<"  " << rotation.m_y << "  " << rotation.m_z << "  " << "\n";
}

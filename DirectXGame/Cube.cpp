#include "Cube.h"
#include "GraphicsEngine.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "IndexBuffer.h"
#include "AppWindow.h"

#include <iostream>

Cube::Cube(string name, void* shaderByteCode, size_t sizeShader) : AGameObject(name)
{
	Vertex quadList[] = {
		//front
		{Vector3D(-0.5f,-0.5f,-0.5f),   vec2(-1,-1)},
		{Vector3D(-0.5f,0.5f,-0.5f),    vec2(-1,1)},
		{ Vector3D(0.5f,0.5f,-0.5f),   vec2(1,1) },
		{ Vector3D(0.5f,-0.5f,-0.5f),     vec2(1,-1) },
		//back
		{ Vector3D(0.5f,-0.5f,0.5f),   vec2(-1,-1) },
		{ Vector3D(0.5f,0.5f,0.5f),    vec2(-1,-1) },
		{ Vector3D(-0.5f,0.5f,0.5f),   vec2(-1,-1) },
		{ Vector3D(-0.5f,-0.5f,0.5f),     vec2(-1,-1) }
	};

	this->vertexBuffer = GraphicsEngine::getInstance()->createVertexBuffer();
	this->vertexBuffer->load(quadList, sizeof(Vertex), ARRAYSIZE(quadList), shaderByteCode, sizeShader);

	unsigned int indexList[] =
	{
		//front
		0,1,2,
		2,3,0,
		//back
		4,5,6,
		6,7,4,
		//top
		1,6,5,
		5,2,1,
		//bottom
		7,0,3,
		3,4,7,
		//right
		3,2,5,
		5,4,3,
		//left
		7,6,1,
		1,0,7
	};

	this->indexBuffer = GraphicsEngine::getInstance()->createIndexBuffer();
	this->indexBuffer->load(indexList, ARRAYSIZE(indexList));

	CBData cbData = {};
	cbData.time = 0;
	this->constantBuffer = GraphicsEngine::getInstance()->createConstantBuffer();
	this->constantBuffer->load(&cbData, sizeof(CBData));

	std::cout << "cube created "<<name<<"\n";
}

Cube::~Cube()
{
	this->vertexBuffer->release();
	this->indexBuffer->release();
	AGameObject::~AGameObject();
}

void Cube::update(float deltaTime)
{
	this->deltaTime = deltaTime;

	this->ticks += deltaTime;

	float rotSpeed = this->ticks * this->speed;
	this->setRotation(rotSpeed, rotSpeed, rotSpeed);
}

void Cube::draw(int width, int height, VertexShader* vertexShader, PixelShader* pixelShader)
{
	GraphicsEngine* graphicsEngine = GraphicsEngine::getInstance();
	DeviceContext* deviceContext = graphicsEngine->getImmediateDeviceContext();

	CBData cbData = {};

	cbData.time = this->ticks;

	if (this->deltaPos > 1.0f)
	{
		this->deltaPos = 0.0f;
	}
	else
	{
		this->deltaPos += this->deltaTime * 0.1f;
	}

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

void Cube::setAnimSpeed(float speed)
{
	this->speed = speed;
}

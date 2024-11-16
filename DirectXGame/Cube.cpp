#include "Cube.h"
#include "GraphicsEngine.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "IndexBuffer.h"
#include "AppWindow.h"

#include <iostream>

#include "LogUtils.h"

Cube::Cube(string name, void* shaderByteCode, size_t sizeShader) : AGameObject(name)
{
	Vertex vertexList[] =
	{
		//X - Y - Z
		//FRONT FACE
		{Vector3D(-0.5f,-0.5f,-0.5f),vec2(0,1)},
		{Vector3D(-0.5f,0.5f,-0.5f),vec2(0,0)},
		{ Vector3D(0.5f,0.5f,-0.5f),vec2(1,0)},
		{ Vector3D(0.5f,-0.5f,-0.5f),vec2(1,1)},

		//TOP FACE
		{ Vector3D(0.5f,0.5f,-0.5f),vec2(1,1)},
		{ Vector3D(0.5f,0.5f,0.5f),vec2(1,0)},
		{ Vector3D(-0.5f,0.5f,0.5f),vec2(0,0)},
		{ Vector3D(-0.5f,0.5f,-0.5f), vec2(0,1)},

		//BACK FACE
		{ Vector3D(0.5f,-0.5f,0.5f),vec2(0,1)},
		{ Vector3D(0.5f,0.5f,0.5f),vec2(0,0)},
		{ Vector3D(-0.5f,0.5f,0.5f),vec2(1,0)},
		{ Vector3D(-0.5f,-0.5f,0.5f),vec2(1,1)},

		//BOTTOM FACE
		{ Vector3D(0.5f, -0.5f,-0.5f),vec2(0,1)},
		{ Vector3D(0.5f, -0.5f,0.5f),vec2(0,0)},
		{ Vector3D(-0.5f,-0.5f,0.5f),vec2(1,0)},
		{ Vector3D(-0.5f,-0.5f,-0.5f),vec2(1,1)},

		//This vertex needs to be here or one of the bottom triangles is f*cked
		{ Vector3D(0,0,0),vec2(0,0)},
	};

	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertexList);
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	this->vertexBuffer = GraphicsEngine::getInstance()->getRenderSystem()->createVertexBuffer(vertexList, sizeof(Vertex), ARRAYSIZE(vertexList), shaderByteCode, sizeShader, buff_desc);

	unsigned int indexList[] =
	{
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		//TOP SIDE
		4,7,6,
		6,5,4,
		//BACK SIDE
		8,9,10,
		10,11,8,
		//BOTTOM SIDE
		12,13,14,
		14,15,12,
		//RIGHT SIDE
		2,9,8,
		8,3,2,
		//LEFT SIDE
		1,0,11,
		11,10,1
	};

	this->indexBuffer = GraphicsEngine::getInstance()->getRenderSystem()->createIndexBuffer(indexList, ARRAYSIZE(indexList));

	CBData cbData = {};
	cbData.time = 0;
	this->constantBuffer = GraphicsEngine::getInstance()->getRenderSystem()->createConstantBuffer(&cbData, sizeof(CBData));
}

Cube::~Cube()
{
	AGameObject::~AGameObject();
}

void Cube::update(float deltaTime)
{
	this->deltaTime = deltaTime;

	this->ticks += deltaTime;

	float rotSpeed = this->ticks * this->speed;
	this->setRotation(rotSpeed, rotSpeed, rotSpeed);
}

//void Cube::draw(const RECT clientWindow)
//{
//	AGameObject::draw(clientWindow);
//}

void Cube::setAnimSpeed(float speed)
{
	this->speed = speed;
}

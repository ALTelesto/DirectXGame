#include "RigidBodyCube.h"
#include "GraphicsEngine.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "IndexBuffer.h"

#include "AppWindow.h"

#include "LogUtils.h"

#include "PhysicsComponent.h"
#include "BaseComponentSystem.h"

RigidBodyCube::RigidBodyCube(string name, void* shaderByteCode, size_t sizeShader) : AGameObject(name)
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

RigidBodyCube::~RigidBodyCube()
{
}

void RigidBodyCube::update(float deltaTime)
{
}

void RigidBodyCube::draw(const RECT clientWindow)
{
	if (!constantBuffer || !indexBuffer || !vertexBuffer || !material->pixelShader) {
		LogUtils::log(this->name + " failed draw");
		return;
	}

	int width = clientWindow.right - clientWindow.left;
	int height = clientWindow.bottom - clientWindow.top;

	GraphicsEngine* graphicsEngine = GraphicsEngine::getInstance();
	DeviceContextPtr deviceContext = graphicsEngine->getRenderSystem()->getImmediateDeviceContext();

	CBData cbData = {};

	cbData.worldMatrix = this->localMatrix; // Use the updated physics-based localMatrix
	cbData.viewMatrix = SceneCameraHandler::getInstance()->getSceneCameraViewMatrix();
	cbData.projMatrix = AppWindow::getInstance()->getProjection();

	cbData.cameraPos = SceneCameraHandler::getInstance()->sceneCamera->getLocalPosition();

	cbData.color = Vector3D(material->color.m_x, material->color.m_y, material->color.m_z);
	cbData.metallic = material->metallic;
	cbData.smoothness = material->smoothness;
	cbData.flatness = material->flatness;
	cbData.tiling = material->tiling;
	cbData.offset = material->offset;

	cbData.hasAlbedoMap = material->albedoTexture != nullptr;
	cbData.hasMetallicMap = material->metallicTexture != nullptr;
	cbData.hasSmoothnessMap = material->smoothnessTexture != nullptr;
	cbData.hasNormalMap = material->normalTexture != nullptr;

	this->constantBuffer->update(deviceContext, &cbData);
	deviceContext->setConstantBuffer(material->vertexShader, this->constantBuffer);
	deviceContext->setConstantBuffer(material->pixelShader, this->constantBuffer);

	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(material->vertexShader);
	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(material->pixelShader);

	deviceContext->setIndexBuffer(this->indexBuffer);
	deviceContext->setVertexBuffer(this->vertexBuffer);

	deviceContext->setTextures(material);

	deviceContext->drawIndexedTriangleList(this->indexBuffer->getSizeIndexList(), 0, 0);
}

void RigidBodyCube::updateLocalMatrix()
{
	AGameObject::updateLocalMatrix();

	PhysicsComponent* component = dynamic_cast<PhysicsComponent*>(this->findComponentOfType(AComponent::Physics));
	if (component != nullptr) component->setTransform();
}

#include "MeshedObject.h"
#include "GraphicsEngine.h"
#include "LogUtils.h"

#include "Mesh.h"

MeshedObject::MeshedObject(string name, wstring meshName) : AGameObject(name)
{
	this->mesh = GraphicsEngine::getInstance()->getMeshManager()->createMeshFromFile(meshName.data());
	if (!this->mesh) LogUtils::log(this, "Meshed object not loaded - " + name);

	this->vertexBuffer = this->mesh->getVertexBuffer();
	this->indexBuffer = this->mesh->getIndexBuffer();

	CBData cbData = {};
	cbData.time = 0;
	this->constantBuffer = GraphicsEngine::getInstance()->getRenderSystem()->createConstantBuffer(&cbData, sizeof(CBData));
}

MeshedObject::MeshedObject(string name, MeshPtr mesh) : AGameObject(name)
{
	this->mesh = mesh;

	if (!this->mesh) LogUtils::log(this, "Meshed object not loaded - " + name);
	this->vertexBuffer = this->mesh->getVertexBuffer();
	this->indexBuffer = this->mesh->getIndexBuffer();

	CBData cbData = {};
	cbData.time = 0;
	this->constantBuffer = GraphicsEngine::getInstance()->getRenderSystem()->createConstantBuffer(&cbData, sizeof(CBData));
}

void MeshedObject::update(float deltaTime)
{
	if (!hasSetBaseScale)
	{
		this->baseScale = this->getLocalScale().m_x;
		hasSetBaseScale = true;
	}

	this->deltaTime = deltaTime;

	this->ticks += deltaTime;

	float scale = abs(sin(this->ticks)) + baseScale;
	this->setScale(scale, scale, scale);
}

void MeshedObject::draw(const RECT clientWindow)
{
	AGameObject::draw(clientWindow);
}

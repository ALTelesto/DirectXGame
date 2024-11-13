#include "AGameObject.h"

#include "AppWindow.h"
#include "LogUtils.h"

AGameObject::AGameObject(string name)
{
	this->name = name;
	this->localPosition = Vector3D::zeros();
	this->localRotation = Vector3D::zeros();
	this->localScale = Vector3D::zeros();
	this->localMatrix.setIdentity();
}

AGameObject::~AGameObject()
{
}

void AGameObject::draw(const RECT clientWindow)
{
	if (!constantBuffer || !indexBuffer || !vertexBuffer || !material->pixelShader) return;

	int width = clientWindow.right - clientWindow.left;
	int height = clientWindow.bottom - clientWindow.top;

	GraphicsEngine* graphicsEngine = GraphicsEngine::getInstance();
	DeviceContextPtr deviceContext = graphicsEngine->getRenderSystem()->getImmediateDeviceContext();

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

	Matrix4x4 temp = SceneCameraHandler::getInstance()->getSceneCameraViewMatrix();
	cbData.viewMatrix = temp;
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

void AGameObject::setActive(bool active)
{
	this->active = active;
}

bool AGameObject::isActive()
{
	return this->active;
}

void AGameObject::setPosition(float x, float y, float z)
{
	this->localPosition = Vector3D(x, y, z);
}

void AGameObject::setPosition(Vector3D pos)
{
	this->localPosition = pos;
}

Vector3D AGameObject::getLocalPosition()
{
	return this->localPosition;
}

void AGameObject::setScale(float x, float y, float z)
{
	this->localScale = Vector3D(x, y, z);
}

void AGameObject::setScale(Vector3D scale)
{
	this->localScale = scale;
}

Vector3D AGameObject::getLocalScale()
{
	return this->localScale;
}

void AGameObject::setRotation(float x, float y, float z)
{
	this->localRotation = Vector3D(x, y, z);
}

void AGameObject::setRotation(Vector3D rot)
{
	this->localPosition = rot;
}

Vector3D AGameObject::getLocalRotation()
{
	return this->localRotation;
}

string AGameObject::getName()
{
	return this->name;
}

void AGameObject::setMaterial(MaterialPtr material)
{
	LogUtils::log(this,"set material");
	this->material = material;
}

MaterialPtr AGameObject::getMaterial()
{
	return material;
}

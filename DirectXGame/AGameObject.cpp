#include "AGameObject.h"

#include <cmath>

#include "AppWindow.h"
#include "LogUtils.h"

#include "EditorAction.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846 // Define pi if not defined
#endif

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
	if (!constantBuffer || !indexBuffer || !vertexBuffer || !material->pixelShader)
	{
		LogUtils::log(this->name + " failed draw");
		return;
	}

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

	cbData.worldMatrix = this->localMatrix;

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
	updateLocalMatrix();
}

void AGameObject::setPosition(Vector3D pos)
{
	this->localPosition = pos;
	updateLocalMatrix();
}

Vector3D AGameObject::getLocalPosition()
{
	return this->localPosition;
}

void AGameObject::setScale(float x, float y, float z)
{
	this->localScale = Vector3D(x, y, z);
	updateLocalMatrix();
}

void AGameObject::setScale(Vector3D scale)
{
	this->localScale = scale;
	updateLocalMatrix();
}

Vector3D AGameObject::getLocalScale()
{
	return this->localScale;
}

void AGameObject::setRotation(float x, float y, float z)
{
	float radX = x * static_cast<float>(M_PI) / 180.0f;
	float radY = y * static_cast<float>(M_PI) / 180.0f;
	float radZ = z * static_cast<float>(M_PI) / 180.0f;
	this->localRotation = Vector3D(radX, radY, radZ);
	updateLocalMatrix();
}

void AGameObject::setRotation(Vector3D rot)
{
	Vector3D radRot(rot.x * static_cast<float>(M_PI) / 180.0f,
		rot.y * static_cast<float>(M_PI) / 180.0f,
		rot.z * static_cast<float>(M_PI) / 180.0f);
	this->localRotation = radRot;
	updateLocalMatrix();
}

Vector3D AGameObject::getLocalRotation()
{
	return this->localRotation;
}

void AGameObject::updateLocalMatrix()
{
	this->localMatrix.setIdentity();
	this->localMatrix.setScale(this->getLocalScale());

	Matrix4x4 rotationMatrix;
	rotationMatrix.setIdentity();

	rotationMatrix.setRotationZ(this->getLocalRotation().z);
	this->localMatrix *= rotationMatrix;

	rotationMatrix.setIdentity();
	rotationMatrix.setRotationY(this->getLocalRotation().y);
	this->localMatrix *= rotationMatrix;

	rotationMatrix.setIdentity();
	rotationMatrix.setRotationX(this->getLocalRotation().x);
	this->localMatrix *= rotationMatrix;

	Matrix4x4 translationMatrix;
	translationMatrix.setIdentity();
	translationMatrix.setTranslation(this->getLocalPosition());
	this->localMatrix *= translationMatrix;
}

void AGameObject::setLocalMatrix(Matrix4x4 matrix)
{
	this->localMatrix = matrix;
}

Matrix4x4 AGameObject::getLocalMatrix()
{
	return this->localMatrix;
}

void AGameObject::setName(string name)
{
	this->name = name;
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

void AGameObject::setPhysicsLocalMatrix(float physicsLocalMatrix[16])
{
	float transposedMatrix[16];
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			transposedMatrix[row * 4 + col] = physicsLocalMatrix[col * 4 + row];
		}
	}
	this->localMatrix.fromFloatArray(transposedMatrix);
}

float* AGameObject::getPhysicsLocalMatrix()
{
	float rowMajorArray[16];
	this->localMatrix.toFloatArray(rowMajorArray);

	static float columnMajorArray[16];
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			columnMajorArray[col * 4 + row] = rowMajorArray[row * 4 + col];
		}
	}

	LogUtils::log("column");
	for(float x : columnMajorArray)
	{
		LogUtils::log(std::to_string(x));
	}

	LogUtils::log("row");
	for (float x : rowMajorArray)
	{
		LogUtils::log(std::to_string(x));
	}

	return columnMajorArray;
}

void AGameObject::attachComponent(AComponent* component)
{
	this->componentList.push_back(component);
}

void AGameObject::detachComponent(AComponent* component)
{
	int index = -1;
	for (int i = 0; i < this->componentList.size(); i++)
	{
		if (this->componentList[i] == component)
		{
			index = i;
			break;
		}
	}

	if (index != -1)
	{
		this->componentList.erase(this->componentList.begin() + index);
	}
	else
	{
		LogUtils::log(this, "No component detached: component not found");
	}
}

AComponent* AGameObject::findComponentByName(std::string name)
{
	for (AComponent* component : componentList)
	{
		if (component->getName() == name)
		{
			return component;
		}
	}
	return nullptr;
}

AComponent* AGameObject::findComponentOfType(AComponent::ComponentType type)
{
	for (AComponent* component : componentList)
	{
		if (component->getType() == type)
		{
			return component;
		}
	}
	return nullptr;
}

ComponentList AGameObject::getComponentsOfType(AComponent::ComponentType type)
{
	ComponentList foundComponentsList;
	for(AComponent* component : componentList)
	{
		if(component->getType() == type)
		{
			foundComponentsList.push_back(component);
		}
	}
	return foundComponentsList;
}

void AGameObject::setState(EditorAction* action)
{
	this->setPosition(action->getStoredPos());
	this->setRotation(action->getStoredRotation());
	this->setScale(action->getStoredScale());
	this->setLocalMatrix(action->getStoredMatrix());
}

void AGameObject::awake()
{
}

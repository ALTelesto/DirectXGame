#pragma once
#include <string>
#include <reactphysics3d/mathematics/Transform.h>

#include "Vector3D.h"
#include "Matrix4x4.h"
#include "Prerequisites.h"

#include "MaterialResource.h"

#include "AComponent.h"
#include "PhysicsSystem.h"

using namespace std;

class AGameObject
{
public:
	AGameObject(string name);
	~AGameObject();

	virtual void update(float deltaTime) = 0;
	virtual void draw(const RECT clientWindow);

	void setActive(bool active);
	bool isActive();

	void setPosition(float x, float y, float z);
	void setPosition(Vector3D pos);
	Vector3D getLocalPosition();

	void setScale(float x, float y, float z);
	void setScale(Vector3D scale);
	Vector3D getLocalScale();

	void setRotation(float x, float y, float z);
	void setRotation(Vector3D rot);
	Vector3D getLocalRotation();

	virtual void updateLocalMatrix();
	void setLocalMatrix(Matrix4x4 matrix);
	Matrix4x4 getLocalMatrix();

	void setName(string name);
	string getName();

	void setMaterial(MaterialPtr material);
	MaterialPtr getMaterial();

	void setPhysicsLocalMatrix(float physicsLocalMatrix[16]);
	float* getPhysicsLocalMatrix();

	void attachComponent(AComponent* component);
	void detachComponent(AComponent* component);

	AComponent* findComponentByName(std::string name);
	AComponent* findComponentOfType(AComponent::ComponentType type);
	ComponentList getComponentsOfType(AComponent::ComponentType type);

	struct vec2 {
		vec2(float _x, float _y):x(_x), y(_y){}

		float x, y;
	};

	struct Vertex
	{
		Vector3D position;
		vec2 texCoord;
	};

	_declspec(align(16))
	struct CBData
	{
		Matrix4x4 worldMatrix;
		Matrix4x4 viewMatrix;
		Matrix4x4 projMatrix;
		float time;

		Vector3D cameraPos;

		Vector3D color;
		float metallic;
		float smoothness;
		float flatness;
		Vector2D tiling;
		Vector2D offset;

		float hasAlbedoMap;
		float hasNormalMap;
		float hasMetallicMap;
		float hasSmoothnessMap;
	};

	void setState(EditorAction* action);

protected:
	bool active = true;

	VertexBufferPtr vertexBuffer;
	IndexBufferPtr indexBuffer;
	ConstantBufferPtr constantBuffer;

	float ticks = 0.0f;
	float deltaPos = 0.0f;
	float deltaTime = 0.0f;
	float speed = 10.0f;

	string name;
	Vector3D localPosition;
	Vector3D localScale;
	Vector3D localRotation;
	Matrix4x4 localMatrix;

	MaterialPtr material;

	ComponentList componentList;

	virtual void awake();
};


#pragma once
#include <string>
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "Prerequisites.h"

#include "Material.h"

using namespace std;

class AGameObject
{
public:
	AGameObject(string name);
	~AGameObject();

	virtual void update(float deltaTime) = 0;
	virtual void draw(int width, int height, VertexShaderPtr vertexShader, PixelShaderPtr pixelShader) = 0;
	virtual void draw(const RECT clientWindow) = 0;

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

	string getName();

	void setMaterial(MaterialPtr material);
	MaterialPtr getMaterial();

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
	};

protected:
	bool active = true;

	string name;
	Vector3D localPosition;
	Vector3D localScale;
	Vector3D localRotation;
	Matrix4x4 localMatrix;

	MaterialPtr material;
};


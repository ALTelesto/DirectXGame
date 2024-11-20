#pragma once
#include "AGameObject.h"
class RigidBodyCube :
    public AGameObject
{
public:
	RigidBodyCube(string name, void* shaderByteCode, size_t sizeShader);
	~RigidBodyCube();

	void update(float deltaTime) override;
	void draw(const RECT clientWindow) override;

	void updateLocalMatrix() override;
};


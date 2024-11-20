#pragma once
#include "AGameObject.h"
class RigidBodyPlane :
    public AGameObject
{
public:
    RigidBodyPlane(string name, void* shaderByteCode, size_t sizeShader);
    ~RigidBodyPlane();

    void update(float deltaTime) override;

    void updateLocalMatrix() override;
};


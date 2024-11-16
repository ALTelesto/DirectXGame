#pragma once
#include "AComponent.h"
#include <reactphysics3d/reactphysics3d.h>
using namespace reactphysics3d;
class PhysicsComponent :
    public AComponent
{
public:
    PhysicsComponent(std::string name, AGameObject* owner);
    ~PhysicsComponent() override;
    void perform(float deltaTime) override;
    RigidBody* getRigidBody();

private:
    float mass = 1000.0f;
    RigidBody* rigidBody;
};


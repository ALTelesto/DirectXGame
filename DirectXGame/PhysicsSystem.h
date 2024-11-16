#pragma once
#include "BaseSystem.h"
#include "Prerequisites.h"
#include "reactphysics3d/reactphysics3d.h"
using namespace reactphysics3d;
class PhysicsComponent;
class PhysicsSystem : public BaseSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem() override;

	void updateAllComponents() override;
	PhysicsWorld* getPhysicsWorld();
	PhysicsCommon* getPhysicsCommon();

private:
	PhysicsCommon* physicsCommon;
	PhysicsWorld* physicsWorld;
};


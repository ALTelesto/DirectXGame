#include "PhysicsSystem.h"

#include "PhysicsComponent.h"

#include "EngineTime.h"
#include "LogUtils.h"

PhysicsSystem::PhysicsSystem()
{
	this->physicsCommon = new PhysicsCommon();
	PhysicsWorld::WorldSettings settings;
	settings.defaultVelocitySolverNbIterations = 50;
	settings.gravity = Vector3(0, -9.81, 0);
	this->physicsWorld = this->physicsCommon->createPhysicsWorld(settings);
	LogUtils::log(this, "Successfully created physics world");
}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::updateAllComponents()
{
	if(EngineTime::getDeltaTime() > 0.0f)
	{
		this->physicsWorld->update(EngineTime::getDeltaTime());
		for(int i = 0; i < this->componentList.size(); i++)
		{
			this->componentList[i]->perform(EngineTime::getDeltaTime());
		}
	}
}

PhysicsWorld* PhysicsSystem::getPhysicsWorld()
{
	return this->physicsWorld;
}

PhysicsCommon* PhysicsSystem::getPhysicsCommon()
{
	return this->physicsCommon;
}

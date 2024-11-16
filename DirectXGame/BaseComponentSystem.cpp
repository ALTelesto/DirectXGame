#include "BaseComponentSystem.h"

BaseComponentSystem* BaseComponentSystem::sharedInstance = nullptr;

BaseComponentSystem* BaseComponentSystem::getInstance()
{
	return sharedInstance;
}

void BaseComponentSystem::initialize()
{
	if (BaseComponentSystem::sharedInstance) throw std::exception("BaseComponentSystem already initialized");
	sharedInstance = new BaseComponentSystem();
}

void BaseComponentSystem::destroy()
{
	if (sharedInstance != NULL)
	{
		delete sharedInstance;
	}
}

PhysicsSystem* BaseComponentSystem::getPhysicsSystem()
{
	return this->physicsSystem;
}

BaseComponentSystem::BaseComponentSystem()
{
	this->physicsSystem = new PhysicsSystem();
}

BaseComponentSystem::~BaseComponentSystem()
{
}

#include "AComponent.h"

#include "AGameObject.h"

AComponent::AComponent(std::string name, ComponentType type, AGameObject* owner) : name(name), type(type), owner(owner)
{
}

AComponent::~AComponent()
{
}

void AComponent::attachOwner(AGameObject* owner)
{
	this->owner = owner;
	owner->attachComponent(this);
}

void AComponent::detachOwner()
{
	if (!owner) return;
	owner->detachComponent(this);
	this->owner = nullptr;
}

AGameObject* AComponent::getOwner()
{
	return this->owner;
}

AComponent::ComponentType AComponent::getType()
{
	return this->type;
}

std::string AComponent::getName()
{
	return this->name;
}

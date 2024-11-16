#pragma once

#include "Prerequisites.h"

class AGameObject;
class AComponent
{
public:
	enum ComponentType {None = -1, Script = 0, Renderer = 1, Input = 2, Physics = 3};
	AComponent(std::string name, ComponentType type, AGameObject* owner);
	virtual ~AComponent();

	void attachOwner(AGameObject* owner);
	void detachOwner();

	AGameObject* getOwner();
	ComponentType getType();

	std::string getName();

	virtual void perform(float deltaTime) = 0;

protected:
	AGameObject* owner;
	ComponentType type;
	std::string name;
};


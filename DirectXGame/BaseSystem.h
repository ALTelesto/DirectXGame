#pragma once

#include "Prerequisites.h"

class AComponent;

typedef std::unordered_map<std::string, AComponent*> ComponentTable;
typedef std::vector<AComponent*> ComponentList;

class BaseSystem
{
public:
	BaseSystem();
	virtual ~BaseSystem();

	void registerComponent(AComponent* component);
	void unregisterComponent(AComponent* component);
	void unregisterComponentByName(std::string name);

	AComponent* findComponentByName(std::string name);
	ComponentList getAllComponents();

	virtual void updateAllComponents();

protected:
	ComponentTable componentTable;
	ComponentList componentList;
};


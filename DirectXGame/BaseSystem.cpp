#include "BaseSystem.h"
#include "AComponent.h"
#include "EngineTime.h"
#include "LogUtils.h"

BaseSystem::BaseSystem()
{
}

BaseSystem::~BaseSystem()
{
}

void BaseSystem::registerComponent(AComponent* component)
{
	this->componentTable[component->getName()] = component;
	this->componentList.push_back(component);
}

void BaseSystem::unregisterComponent(AComponent* component)
{
	if (this->componentTable[component->getName()] != NULL)
	{
		this->componentTable.erase(component->getName());
		int index = -1;
		for (int i = 0; i < this->componentList.size(); i++)
		{
			if (this->componentList[i] == component)
			{
				index = i;
				break;
			}
		}

		if (index != -1)
		{
			this->componentList.erase(this->componentList.begin() + index);
		}
	}
	else
	{
		LogUtils::log(this, "No component unregistered: component not registered");
	}
}

void BaseSystem::unregisterComponentByName(std::string name)
{
	if (this->componentTable[name] != NULL)
	{
		this->unregisterComponent(this->componentTable[name]);
	}
	else
	{
		LogUtils::log(this, "No component unregistered: component name not found");
	}
}

AComponent* BaseSystem::findComponentByName(std::string name)
{
	if (this->componentTable[name] == NULL)
	{
		LogUtils::log(this, "No Physics component found: component not registered");
	}
	return this->componentTable[name];
}

ComponentList BaseSystem::getAllComponents()
{
	return this->componentList;
}

void BaseSystem::updateAllComponents()
{
	if (EngineTime::getDeltaTime() > 0.0f)
	{
		for (int i = 0; i < this->componentList.size(); i++)
		{
			this->componentList[i]->perform(EngineTime::getDeltaTime());
		}
	}
}

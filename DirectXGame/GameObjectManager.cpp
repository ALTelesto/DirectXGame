#include "GameObjectManager.h"

#include "EditorAction.h"

#include "LogUtils.h"

GameObjectManager* GameObjectManager::sharedInstance = nullptr;

GameObjectManager* GameObjectManager::getInstance()
{
	return sharedInstance;
}

void GameObjectManager::initialize()
{
	if (GameObjectManager::sharedInstance) throw std::exception("GameObjectManager already initialized");
	sharedInstance = new GameObjectManager();
}

void GameObjectManager::destroy()
{
	sharedInstance->gameObjectMap.clear();
	sharedInstance->gameObjectList.clear();

	delete sharedInstance;
}

GameObjectPtr GameObjectManager::findObjectByName(const std::string& name)
{
	return gameObjectMap[name];
}

GameObjectList GameObjectManager::getAllObjects()
{
	return gameObjectList;
}

GameObjectHashMap GameObjectManager::getObjectHashMap()
{
	return gameObjectMap;
}

int GameObjectManager::activeObjects() const
{
	return static_cast<int>(gameObjectList.size());
}

void GameObjectManager::updateAll(float deltaTime)
{
	for(GameObjectPtr gameObject : gameObjectList)
	{
		if (!gameObject->isActive()) continue;

		gameObject->update(deltaTime);
	}
}

void GameObjectManager::drawAll(const RECT clientWindow)
{
	for (GameObjectPtr gameObject : gameObjectList)
	{
		if (!gameObject->isActive()) continue;
		//LogUtils::log(gameObject->getName());
		gameObject->draw(clientWindow);
	}
}

void GameObjectManager::addObject(GameObjectPtr gameObject)
{
	int	duplicates = 0;
	std::string name = gameObject->getName();
	std::string finalName;
	while (!gameObjectMap.insert(std::make_pair(name, gameObject)).second)
	{
		name = gameObject->getName() + " (" + std::to_string(duplicates) + ")";
		duplicates++;
	}
	if(duplicates != 0)
	{
		finalName = gameObject->getName() + " (" + std::to_string(duplicates-1) + ")";
		gameObject->setName(finalName);
	}
	this->gameObjectList.push_back(gameObject);
}

void GameObjectManager::deleteObject(GameObjectPtr gameObject)
{
	gameObjectMap.erase(gameObject->getName());
	GameObjectList::iterator it = std::find(gameObjectList.begin(), gameObjectList.end(), gameObject);

	if (it != gameObjectList.end())
	{
		gameObjectList.erase(it);
	}
}

void GameObjectManager::deleteObjectByName(const std::string& name)
{
	deleteObject(findObjectByName(name));
}

void GameObjectManager::setSelectedObject(const std::string& name)
{
	if (!gameObjectMap[name])
		return;

	setSelectedObject(gameObjectMap[name]);
}

void GameObjectManager::setSelectedObject(GameObjectPtr gameObject)
{
	selectedObject = gameObject;
}

GameObjectPtr GameObjectManager::getSelectedObject()
{
	return selectedObject;
}

void GameObjectManager::saveEditStates()
{
	while(!editStates.empty())
	{
		editStates.pop();
	}
	for(GameObjectPtr gameObject : gameObjectList)
	{
		EditorAction* action = new EditorAction(gameObject);
		editStates.push(action);
	}
}

void GameObjectManager::restoreEditStates()
{
	while (!editStates.empty())
	{
		EditorAction* action = editStates.top();
		editStates.pop();

		GameObjectPtr gameObject = this->findObjectByName(action->getOwnerName());
		if(gameObject != nullptr)
		{
			gameObject->setState(action);
		}
		else
		{
			LogUtils::log(this, "Failed to restore " + action->getOwnerName());
		}
	}
}

GameObjectManager::GameObjectManager() = default;

GameObjectManager::~GameObjectManager(){}

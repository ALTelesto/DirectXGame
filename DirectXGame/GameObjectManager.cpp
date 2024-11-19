#include "GameObjectManager.h"

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
	while (!gameObjectMap.insert(std::make_pair(name, gameObject)).second)
	{
		name = gameObject->getName() + " (" + std::to_string(duplicates) + ")";
		duplicates++;
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

GameObjectManager::GameObjectManager() = default;

GameObjectManager::~GameObjectManager(){}

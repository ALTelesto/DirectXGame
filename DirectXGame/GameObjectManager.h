#pragma once
#include "Prerequisites.h"

#include "AGameObject.h"
#include "EPrimitive.h"

typedef std::vector<GameObjectPtr> GameObjectList;
typedef std::unordered_map<std::string, GameObjectPtr> GameObjectHashMap;

class GameObjectManager
{
public:
	static GameObjectManager* getInstance();
	static void initialize();
	static void destroy();

public:
	GameObjectPtr findObjectByName(const std::string& name);
	GameObjectList getAllObjects();
	int activeObjects() const;

	void updateAll(float deltaTime);
	void drawAll(const RECT clientWindow);

	void addObject(GameObjectPtr gameObject);
	void deleteObject(GameObjectPtr gameObject);
	void deleteObjectByName(const std::string& name);

	void setSelectedObject(const std::string& name);
	void setSelectedObject(GameObjectPtr gameObject);
	GameObjectPtr getSelectedObject();

private:
	GameObjectManager();
	~GameObjectManager();
	GameObjectManager(GameObjectManager const&) {};
	GameObjectManager& operator = (GameObjectManager const&) {};
	static GameObjectManager* sharedInstance;

	GameObjectHashMap gameObjectMap;
	GameObjectList gameObjectList;

	GameObjectPtr selectedObject = nullptr;
};


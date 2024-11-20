#pragma once
#include "Prerequisites.h"

#include "AGameObject.h"
#include "EPrimitive.h"

class GameObjectManager
{
public:
	static GameObjectManager* getInstance();
	static void initialize();
	static void destroy();

public:
	GameObjectPtr findObjectByName(const std::string& name);
	GameObjectList getAllObjects();
	GameObjectHashMap getObjectHashMap();
	int activeObjects() const;

	void updateAll(float deltaTime);
	void drawAll(const RECT clientWindow);

	void addObject(GameObjectPtr gameObject);
	void deleteObject(GameObjectPtr gameObject);
	void deleteObjectByName(const std::string& name);

	void setSelectedObject(const std::string& name);
	void setSelectedObject(GameObjectPtr gameObject);
	GameObjectPtr getSelectedObject();

	void saveEditStates();
	void restoreEditStates();

private:
	GameObjectManager();
	~GameObjectManager();
	GameObjectManager(GameObjectManager const&) {};
	GameObjectManager& operator = (GameObjectManager const&) {};
	static GameObjectManager* sharedInstance;

	GameObjectHashMap gameObjectMap;
	GameObjectList gameObjectList;

	ActionStack editStates;

	GameObjectPtr selectedObject = nullptr;
};


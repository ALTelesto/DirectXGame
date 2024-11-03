#include "SceneCameraHandler.h"

#include <inttypes.h>

#include "EngineTime.h"

SceneCameraHandler* SceneCameraHandler::sharedInstance = nullptr;

SceneCameraHandler* SceneCameraHandler::getInstance()
{
	return sharedInstance;
}

void SceneCameraHandler::initialize()
{
	if (sharedInstance == nullptr) sharedInstance = new SceneCameraHandler();
	sharedInstance->sceneCamera = new Camera("Camera");
}

void SceneCameraHandler::destroy()
{
	if (sharedInstance != NULL)
	{
		delete sharedInstance;
	}
}

void SceneCameraHandler::update()
{
	if (sceneCamera != nullptr) sceneCamera->update(EngineTime::getDeltaTime());
}

void SceneCameraHandler::onFocus()
{
	if (sceneCamera != nullptr) sceneCamera->onFocus();
}

void SceneCameraHandler::onKillFocus()
{
	if (sceneCamera != nullptr) sceneCamera->onKillFocus();
}

Matrix4x4 SceneCameraHandler::getSceneCameraViewMatrix()
{
	return this->sceneCamera->getViewMatrix();
}

SceneCameraHandler::SceneCameraHandler()
{
}

SceneCameraHandler::~SceneCameraHandler()
{
}

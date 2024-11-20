#include "EngineBackend.h"

#include "GameObjectManager.h"

EngineBackend* EngineBackend::sharedInstance = nullptr;

EngineBackend* EngineBackend::getInstance()
{
	return sharedInstance;
}

void EngineBackend::initialize()
{
	if (EngineBackend::sharedInstance) throw std::exception("EngineBackend already initialized");
	sharedInstance = new EngineBackend();
}

void EngineBackend::destroy()
{
	delete sharedInstance;
}

void EngineBackend::setMode(EditorMode mode)
{
	this->editorMode = mode;
	if(this->editorMode == EditorMode::PLAY)
	{
		GameObjectManager::getInstance()->saveEditStates();
	}
	else if(this->editorMode == EditorMode::EDITOR)
	{
		GameObjectManager::getInstance()->restoreEditStates();
	}
}

void EngineBackend::startFrameStep()
{
	this->frameStepping = true;
}

void EngineBackend::endFrameStep()
{
	this->frameStepping = false;
}

bool EngineBackend::insideFrameStep()
{
	return this->frameStepping;
}

EngineBackend::EditorMode EngineBackend::getMode()
{
	return this->editorMode;
}

EngineBackend::EngineBackend()
{
}

EngineBackend::~EngineBackend()
{
}

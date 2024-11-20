#include "ActionHistory.h"

#include "AGameObject.h"
#include "EngineBackend.h"
#include "EditorAction.h"
#include "LogUtils.h"

ActionHistory* ActionHistory::sharedInstance = nullptr;

ActionHistory* ActionHistory::getInstance()
{
	return sharedInstance;
}

void ActionHistory::initialize()
{
	if (ActionHistory::sharedInstance) throw std::exception("ActionHistory already initialized");
	sharedInstance = new ActionHistory();
}

void ActionHistory::destroy()
{
	delete sharedInstance;
}

void ActionHistory::recordAction(GameObjectPtr gameObject)
{
	if(EngineBackend::getInstance()->getMode() == EngineBackend::EDITOR)
	{
		EditorAction* editorAction = new EditorAction(gameObject);
		this->actionsPerformed.push(editorAction);

		LogUtils::log(this, "Stored action " + gameObject->getName());
	}
}

bool ActionHistory::hasRemainingUndoActions()
{
	return !this->actionsPerformed.empty();
}

bool ActionHistory::hasRemainingRedoActions()
{
	return !this->actionsCancelled.empty();
}

EditorAction* ActionHistory::undoAction()
{
	if (EngineBackend::getInstance()->getMode() != EngineBackend::EDITOR) return nullptr;

	if(this->hasRemainingUndoActions())
	{
		EditorAction* action = this->actionsPerformed.top();
		this->actionsPerformed.pop();
		this->actionsCancelled.push(action);
		return action;
	}
	else
	{
		LogUtils::log(this, "No more actions remaining");
		return nullptr;
	}
}

EditorAction* ActionHistory::redoAction()
{
	if (EngineBackend::getInstance()->getMode() != EngineBackend::EDITOR) return nullptr;

	if (this->hasRemainingRedoActions())
	{
		EditorAction* action = this->actionsCancelled.top();
		this->actionsCancelled.pop();
		this->actionsPerformed.push(action);
		return action;
	}
	else
	{
		LogUtils::log(this, "No more actions remaining");
		return nullptr;
	}
}

void ActionHistory::clear()
{
	while(this->hasRemainingUndoActions())
	{
		this->actionsPerformed.pop();
	}
	while(this->hasRemainingRedoActions())
	{
		this->actionsCancelled.pop();
	}
}

ActionHistory::ActionHistory()
{
}

ActionHistory::~ActionHistory()
{
}

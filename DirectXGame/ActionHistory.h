#pragma once

#include "Prerequisites.h"

class EditorAction;
class ActionHistory
{
public:
	static ActionHistory* getInstance();
	static void initialize();
	static void destroy();

	void recordAction(GameObjectPtr gameObject);
	bool hasRemainingUndoActions();
	bool hasRemainingRedoActions();
	EditorAction* undoAction();
	EditorAction* redoAction();
	void clear();

private:
	ActionHistory();
	~ActionHistory();
	ActionHistory(ActionHistory const&) {};
	ActionHistory& operator=(ActionHistory const&) {};

	static ActionHistory* sharedInstance;

	ActionStack actionsPerformed;
	ActionStack actionsCancelled;
};


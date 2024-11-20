#include "EditorAction.h"

#include "AGameObject.h"

EditorAction::EditorAction(GameObjectPtr gameObject) :
	objectName(gameObject->getName()),
	localPosition(gameObject->getLocalPosition()),
	localRotation(gameObject->getLocalRotation()),
	localScale(gameObject->getLocalScale()),
	localMatrix(gameObject->getLocalMatrix())
{
}

EditorAction::~EditorAction()
{
}

std::string EditorAction::getOwnerName()
{
	return objectName;
}

Vector3D EditorAction::getStoredPos()
{
	return localPosition;
}

Vector3D EditorAction::getStoredScale()
{
	return localScale;
}

Vector3D EditorAction::getStoredRotation()
{
	return localRotation;
}

Matrix4x4 EditorAction::getStoredMatrix()
{
	return localMatrix;
}

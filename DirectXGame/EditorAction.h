#pragma once
#include "Matrix4x4.h"
#include "Prerequisites.h"
#include "Vector3D.h"

class EditorAction
{
public:
	EditorAction(GameObjectPtr gameObject);
	~EditorAction();

	std::string getOwnerName();
	Vector3D getStoredPos();
	Vector3D getStoredScale();
	Vector3D getStoredRotation();
	Matrix4x4 getStoredMatrix();

private:
	std::string objectName;
	Vector3D localPosition;
	Vector3D localScale;
	Vector3D localRotation;
	Matrix4x4 localMatrix;
};


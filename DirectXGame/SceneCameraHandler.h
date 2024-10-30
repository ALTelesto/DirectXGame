#pragma once
#include "Camera.h"
#include "Matrix4x4.h"

class SceneCameraHandler
{
public:
	static SceneCameraHandler* getInstance();
	static void initialize();
	static void destroy();

	void update();

	void onFocus();
	void onKillFocus();

	void setCamRotation(float x, float y);

	Matrix4x4 getSceneCameraViewMatrix();

private:
	SceneCameraHandler();
	~SceneCameraHandler();
	SceneCameraHandler(SceneCameraHandler const&) {};
	SceneCameraHandler& operator=(SceneCameraHandler const&) {};
	static SceneCameraHandler* sharedInstance;

	Camera* sceneCamera;
};


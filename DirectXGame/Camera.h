#pragma once
#include "AGameObject.h"
#include "InputListener.h"

class Camera : public AGameObject, public InputListener
{
public:
	Camera(string name);
	~Camera();

	void update(float deltaTime) override;

	virtual void draw(const RECT clientWindow) override;

	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& delta_mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;

	void onFocus();
	void onKillFocus();

private:
	bool canMove = false;

	float speed = 0.1f;
	float mouseDown = false;

	float forward = 0.0f;
	float rightward = 0.0f;
	float upward = 0.0f;

	float rot_x = 0.0f;
	float rot_y = 0.0f;
};


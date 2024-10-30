#pragma once
#include "AGameObject.h"
#include "InputListener.h"

class Camera : public AGameObject, public InputListener
{
public:
	Camera(string name);
	~Camera();

	void update(float deltaTime) override;
	Matrix4x4 getViewMatrix();

	virtual void draw(int width, int height, VertexShaderPtr vertexShader, PixelShaderPtr pixelShader) override;

	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& delta_mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;

	void onFocus();
	void onKillFocus();

	void setCamRotation(float x, float y);

private:
	void updateViewMatrix();

	float mouseDown = false;

	float forward = 0.0f;
	float rightward = 0.0f;

	float rot_x = 0.0f;
	float rot_y = 0.0f;
};

